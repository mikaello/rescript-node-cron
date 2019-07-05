# bs-cron

[![Build Status](https://travis-ci.org/mikaello/bs-node-cron.svg?branch=master)](https://travis-ci.org/mikaello/bs-node-cron)

Bindings for [node-cron](https://github.com/kelektiv/node-cron), a tool that allows you to execute _something_ on a schedule. This is typically done using the cron syntax.

## Getting started

```
yarn add mikaello/bs-cron
```

Then add `bs-cron` as a dependency to `bsconfig.json`:

```diff
"bs-dependencies": [
+  "bs-cron"
]
```

## Example

```reason
open BsCron

// Make a job that will fire every second when started
let job =
  CronJob.make(
    `CronString("* * * * * *"),
    _ => Js.log("Just doing my job"),
    (),
  );

// Firing every second, printing 'Just doing my job'
start(job);

let time =
  CronTime.make(`JsDate(Js.Date.fromString("2021-04-11T10:20:30Z")), ());

// setTime will stop the current job, and change when it will fire next
setTime(job, time);

// It will now fire once in april 2021
start(job);
```

## Contribute

- If you find bugs or want to improve this library, feel free to open an issue or PR.
- If you are upgrading any dependencies, please use yarn so `yarn.lock` is updated.
- Try to adhere to [Angular commit guidelines](https://github.com/angular/angular.js/blob/master/DEVELOPERS.md#-git-commit-guideline).
