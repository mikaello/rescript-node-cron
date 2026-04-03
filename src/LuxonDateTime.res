type t

type setOptions = {
  year?: int,
  month?: int,
  day?: int,
  hour?: int,
  minute?: int,
  second?: int,
  millisecond?: int,
}

@module("luxon") @scope("DateTime") external now: unit => t = "now"
@module("luxon") @scope("DateTime") external fromISO: string => t = "fromISO"
@module("luxon") @scope("DateTime") external fromJSDate: Date.t => t = "fromJSDate"

@send external set: (t, setOptions) => t = "set"
@send external toJSDate: t => Date.t = "toJSDate"
@send external toISO: t => Nullable.t<string> = "toISO"
@send external equals: (t, t) => bool = "equals"

type duration
@send
external _diff: (
  t,
  t,
  [
    | #milliseconds
    | #seconds
    | #minutes
    | #hours
    | #days
    | #weeks
    | #months
    | #years
  ],
) => duration = "diff"
@send external _toMillis: duration => float = "toMillis"

let diffMilliseconds = (dt1, dt2) => _toMillis(_diff(dt1, dt2, #milliseconds))

@send external toMillis: t => float = "toMillis"
