module CronJob = {
  type t

  @module("cron") @new
  external make: (
    @unwrap [#CronString(string) | #JsDate(Date.t)],
    (unit => unit) => unit,
    unit => unit,
    Nullable.t<bool>,
    Nullable.t<string>,
    Nullable.t<{..}>,
    Nullable.t<bool>,
    Nullable.t<int>,
    Nullable.t<bool>,
  ) => t = "CronJob"

  let make = (
    cronTime: [#CronString(string) | #JsDate(Date.t)],
    onTick: (unit => unit) => unit,
    ~onComplete: unit => unit=_ => (),
    ~start: option<bool>=?,
    ~timezone: option<string>=?,
    ~context: option<{..}>=?,
    ~runOnInit: option<bool>=?,
    ~utcOffset: option<int>=?, // Could also be string, but that complicates
    ~unrefTimeout: option<bool>=?,
    (),
  ) =>
    make(
      cronTime,
      onTick,
      onComplete,
      Nullable.fromOption(start),
      Nullable.fromOption(timezone),
      Nullable.fromOption(context),
      Nullable.fromOption(runOnInit),
      Nullable.fromOption(utcOffset),
      Nullable.fromOption(unrefTimeout),
    )
}

module CronTime = {
  type t

  @module("cron") @new
  external make: (
    @unwrap [#CronString(string) | #JsDate(Date.t)],
    Nullable.t<string>,
    Nullable.t<int>,
  ) => t = "CronTime"

  let make = (
    cronTime: [#CronString(string) | #JsDate(Date.t)],
    ~timezone: option<string>=?,
    ~utcOffset: option<int>=?,
    (),
  ) => make(cronTime, Nullable.fromOption(timezone), Nullable.fromOption(utcOffset))
}

@module("cron") @val
external sendAt: @unwrap [#CronString(string) | #JsDate(Date.t)] => LuxonDateTime.t = "sendAt"

@module("cron") @val
external timeout: @unwrap [#CronString(string) | #JsDate(Date.t)] => float = "timeout"

@send external start: CronJob.t => unit = "start"
@send external stop: CronJob.t => unit = "stop"
@send external setTime: (CronJob.t, CronTime.t) => unit = "setTime"
@send external lastDate: CronJob.t => Nullable.t<Date.t> = "lastDate"
@send external nextDates: (CronJob.t, int) => array<LuxonDateTime.t> = "nextDates"

let nextDateTimes = (~numberOfDates=1, job: CronJob.t) => nextDates(job, numberOfDates)

let nextJsDates = (~numberOfDates=1, job: CronJob.t) =>
  nextDates(job, numberOfDates)->Array.map(LuxonDateTime.toJSDate)

@send external fireOnTick: CronJob.t => unit = "fireOnTick"

@send
external addCallback: (CronJob.t, (unit => unit) => unit) => unit = "addCallback"
