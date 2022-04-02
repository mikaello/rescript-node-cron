open Jest;
open Expect;

beforeEach(() => Jest.useFakeTimers());
afterEach(() => {
  RescriptJestDateMock.clear();
  Jest.clearAllTimers();
  Jest.useRealTimers();
});

let pastDate = "2010-01-01T12:00:00.000Z";
let futureDate = "2010-01-05T12:00:00.000Z";
let postFutureDate = "2010-01-10T12:00:00.000Z";

describe("creating of CronJob", () => {
  test("creating a CronJob and testing onTick", () => {
    let jobHasTicked = ref(false);
    let onTick = _ => jobHasTicked := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    BsCron.start(job);

    RescriptJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^) |> toEqual(true);
  });

  test("creating a CronJob with onComplete option", () => {
    let jobHasTicked = ref(false);
    let jobHasCompleted = ref(false);

    let onTick = _ => jobHasTicked := true;
    let onComplete = _ => jobHasCompleted := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        ~onComplete,
        (),
      );

    BsCron.start(job);
    BsCron.stop(job);

    expect(jobHasCompleted^) |> toEqual(true);
  });

  test("creating a CronJob with onTick and onComplete as param", () => {
    let jobHasTicked = ref(false);
    let jobHasCompleted = ref(false);

    let onTick = onComplete => {
      jobHasTicked := true;
      onComplete();
    };
    let onComplete = _ => jobHasCompleted := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        ~onComplete,
        (),
      );

    BsCron.start(job);

    RescriptJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^ && jobHasCompleted^) |> toEqual(true);
  });

  test("creating a CronJob and not starting will not trigger onTick", () => {
    let jobHasTicked = ref(false);
    let onTick = _ => jobHasTicked := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let _ =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    RescriptJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^) |> toEqual(false);
  });

  test("creating a CronJob with automatic start of the job", () => {
    let jobHasTicked = ref(false);
    let onTick = _ => jobHasTicked := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let _ =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        ~start=true,
        (),
      );

    RescriptJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^) |> toEqual(true);
  });
});

describe("changing CronJobs", () => {
  test("add callBacks to CronJob", () => {
    let jobHasTicked = ref(false);
    let callbackHasTicked = ref(false);

    let onTick = _ => jobHasTicked := true;
    let callback = _ => callbackHasTicked := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    BsCron.start(job);

    // Adding a callback in addition to the onTick parameter
    BsCron.addCallback(job, callback);

    RescriptJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^ && callbackHasTicked^) |> toEqual(true);
  });

  test("fire all callbacks for a CronJob (without timer expiring)", () => {
    let jobHasTicked = ref(false);
    let callbackHasTicked1 = ref(false);
    let callbackHasTicked2 = ref(false);

    let onTick = _ => jobHasTicked := true;
    let callback1 = _ => callbackHasTicked1 := true;
    let callback2 = _ => callbackHasTicked2 := true;

    RescriptJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    BsCron.start(job);

    BsCron.addCallback(job, callback1);
    BsCron.addCallback(job, callback2);

    // This will fire onTick and all callbacks
    BsCron.fireOnTick(job);

    expect(jobHasTicked^ && callbackHasTicked1^ && callbackHasTicked2^)
    |> toEqual(true);
  });

  test("changing time of CronJob with setTime and a CronTime", () => {
    let onTick = _ => ();

    RescriptJestDateMock.advanceTo(Js.Date.fromString("2010-01-01T12:00:00.000Z"));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    // every January 20th twelve o'clock
    let time = BsCron.CronTime.make(`CronString("0 0 12 20 jan *"), ());

    let nextAssumedTick =
      MomentRe.momentDefaultFormat("2010-01-01T12:00:00.000Z")
      |> MomentRe.Moment.setDate(20)
      |> MomentRe.Moment.setHour(12);

    BsCron.setTime(job, time);

    let nextTick = BsCron.nextMomentDates(job)->Array.get(0);

    expect(MomentRe.Moment.toJSON(nextTick))
    |> toEqual(MomentRe.Moment.toJSON(nextAssumedTick));
  });
});
