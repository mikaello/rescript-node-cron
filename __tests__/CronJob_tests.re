open Jest;
open Expect;

beforeEach(() => Jest.useFakeTimers());
afterEach(() => {
  BsJestDateMock.clear();
  Jest.clearAllTimers();
  Jest.useRealTimers();
});

let pastDate = "2010-01-01T12:00:00.000Z";
let futureDate = "2010-01-05T12:00:00.000Z";
let postFutureDate = "2010-01-10T12:00:00.000Z";

describe("CronJob", () => {
  test("creating a CronJob and testing onTick", () => {
    let jobHasTicked = ref(false);

    let onTick = _ => jobHasTicked := true;

    BsJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        (),
      );

    BsCron.start(job);

    BsJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^) |> toEqual(true);
  });

  test("creating a CronJob with onComplete option", () => {
    let jobHasTicked = ref(false);
    let jobHasCompleted = ref(false);

    let onTick = _ => jobHasTicked := true;
    let onComplete = _ => jobHasCompleted := true;

    BsJestDateMock.advanceTo(Js.Date.fromString(pastDate));

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

    BsJestDateMock.advanceTo(Js.Date.fromString(pastDate));

    let job =
      BsCron.CronJob.make(
        `JsDate(Js.Date.fromString(futureDate)),
        onTick,
        ~onComplete,
        (),
      );

    BsCron.start(job);

    BsJestDateMock.advanceTo(Js.Date.fromString(postFutureDate));
    Jest.runAllTimers();

    expect(jobHasTicked^ && jobHasCompleted^) |> toEqual(true);
  });
});