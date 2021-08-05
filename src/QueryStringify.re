[@bs.module "querystringify"] [@bs.val]
external parseQueryString: string => Js.Dict.t(string) = "parse";

[@bs.module "querystringify"] [@bs.val]
external stringifyQuery: Js.Dict.t(string) => string = "stringify";

[@bs.module "querystringify"] [@bs.val]
external stringifyQueryWithPrefix: (Js.Dict.t(string), bool) => string =
  "stringify";

[@bs.module "querystringify"] [@bs.val]
external stringifyQueryWithPrefixString: (Js.Dict.t(string), string) => string =
  "stringify";
