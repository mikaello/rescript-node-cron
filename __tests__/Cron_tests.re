open Jest;
open Expect;

describe("first test suite", () => {
  test("first test", () =>
    expect(1 == 1) |> toEqual(true)
  );
  test("second test", () =>
    expect(1 == 2) |> toEqual(false)
  );
});