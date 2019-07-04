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

// Do job every 10 minute
let job = CronJob.makeFromString(
    "* 10 * * * *",
    () => { Js.log("Just doing my job") },
)

CronJob.start(job);
```

## Contribute

- If you find bugs or want to improve this library, feel free to open an issue or PR.
- If you are upgrading any dependencies, please use yarn so `yarn.lock` is updated.
- Try to adhere to [Angular commit guidelines](https://github.com/angular/angular.js/blob/master/DEVELOPERS.md#-git-commit-guideline).
