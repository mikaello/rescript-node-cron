open Jest;
open Expect;

open BsJestDateMock;

afterEach(() => clear());

describe("first test suite", () => {
  test("sendAt with cron syntax at minute 10", () => {
    let aDate = Js.Date.fromString("2018-05-27T12:00:00Z");
    advanceTo(aDate);

    let aMomentTenMinutesLater =
      Js.Date.setMinutes(aDate, 10.0)
      |> Js.Date.fromFloat
      |> MomentRe.momentWithDate;

    // At minute 10
    let nextSchedule = BsCron.sendAt(`CronString("* 10 * * * *"));

    expect(MomentRe.Moment.isSame(nextSchedule, aMomentTenMinutesLater))
    |> toBe(true);
  });

  test("sendAt with cron syntax at hour 10", () => {
    let aDate = Js.Date.fromString("2018-05-27T12:00:00Z");
    advanceTo(aDate);

    // Every 10 hours
    let nextSchedule = BsCron.sendAt(`CronString("* */10 * * *"));

    let aMomentWithNext10Hour =
      MomentRe.momentWithDate(aDate) |> MomentRe.Moment.setHour(20);

    expect(MomentRe.Moment.isSame(nextSchedule, aMomentWithNext10Hour))
    |> toBe(true);
  });

  test("timeout", () => {
    let aDate = Js.Date.fromString("2018-05-27T12:00:00Z");
    advanceTo(aDate);

    // Every 10 hours
    let msToNextFire = BsCron.timeout(`CronString("* */10 * * *"));

    let aMomentWithNext10Hour =
      MomentRe.momentWithDate(aDate) |> MomentRe.Moment.setHour(20);

    let msDiff =
      MomentRe.diff(
        aMomentWithNext10Hour,
        MomentRe.momentNow(),
        `milliseconds,
      );

    expect(msToNextFire) |> toEqual(msDiff);
  });
});