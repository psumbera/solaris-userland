"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _lodash = require("devtools/client/shared/vendor/lodash");

let newQueuedSources; /* This Source Code Form is subject to the terms of the Mozilla Public
                       * License, v. 2.0. If a copy of the MPL was not distributed with this
                       * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

let queuedSources;
let currentWork;

async function dispatchNewSources() {
  const sources = queuedSources;
  queuedSources = [];
  currentWork = await newQueuedSources(sources);
}

const queue = (0, _lodash.throttle)(dispatchNewSources, 100);

exports.default = {
  initialize: actions => {
    newQueuedSources = actions.newQueuedSources;
    queuedSources = [];
  },
  queue: source => {
    queuedSources.push(source);
    queue();
  },
  queueSources: sources => {
    if (sources.length > 0) {
      queuedSources = queuedSources.concat(sources);
      queue();
    }
  },

  flush: () => Promise.all([queue.flush(), currentWork]),
  clear: () => {
    queuedSources = [];
    queue.cancel();
  }
};