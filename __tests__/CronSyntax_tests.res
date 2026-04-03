open Jest
open Expect

open RescriptJestDateMock

afterEach(() => clear())

describe("functions for just dealing with crontab syntax", () => {
  test("sendAt with cron syntax at minute 10", () => {
    let aDate = Date.fromString("2018-05-27T12:00:00Z")
    advanceTo(aDate)

    let aMomentTenMinutesLater = LuxonDateTime.fromJSDate(aDate)->LuxonDateTime.set({minute: 10})

    // At minute 10
    let nextSchedule = RescriptCron.sendAt(#CronString("* 10 * * * *"))

    expect(LuxonDateTime.toMillis(nextSchedule))->toEqual(
      LuxonDateTime.toMillis(aMomentTenMinutesLater),
    )
  })

  test("sendAt with cron syntax at hour 10", () => {
    let aDate = Date.fromString("2018-05-27T12:00:00Z")
    advanceTo(aDate)

    // Every 10 hours
    let nextSchedule = RescriptCron.sendAt(#CronString("* */10 * * *"))

    let aMomentWithNext10Hour = LuxonDateTime.fromJSDate(aDate)->LuxonDateTime.set({hour: 20})

    expect(LuxonDateTime.toMillis(nextSchedule))->toEqual(
      LuxonDateTime.toMillis(aMomentWithNext10Hour),
    )
  })

  test("sendAt with Js.date instead of cron syntax", () => {
    let aDate = Date.fromString("2010-01-27T12:12:00Z")
    advanceTo(aDate)

    let futureDate = "2040-05-27T12:00:00.000Z"

    // Future Js.Date
    let nextSchedule = RescriptCron.sendAt(#JsDate(Date.fromString(futureDate)))

    expect(Date.toISOString(LuxonDateTime.toJSDate(nextSchedule)))->toBe(futureDate)
  })

  test("timeout", () => {
    let aDate = Date.fromString("2018-05-27T12:00:00Z")
    advanceTo(aDate)

    // Every 10 hours
    let msToNextFire = RescriptCron.timeout(#CronString("* */10 * * *"))

    let aMomentWithNext10Hour = LuxonDateTime.fromJSDate(aDate)->LuxonDateTime.set({hour: 20})

    let msDiff = LuxonDateTime.diffMilliseconds(aMomentWithNext10Hour, LuxonDateTime.now())

    expect(msToNextFire)->toEqual(msDiff)
  })
})
