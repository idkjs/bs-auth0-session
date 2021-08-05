type pair;

// [@bs.get] external provider: pair => ReasonReact.reactClass = "Provider";
// [@bs.get] external consumer: pair => ReasonReact.reactClass = "Consumer";

// [@bs.module "react"] external createContext: 'a => pair = "";

module MakePair = (Config: {
                     type t;
                     let defaultValue: t;
                   }) => {
  type t = Config.t;
  let pair = React.createContext(Config.defaultValue);

  module Provider = {
    let makeProps = (~value: Config.t, ~children, ()) => {
      "value": value,
      "children": children,
    };
    let make = React.Context.provider(pair);
    // let make = (~value: Config.t, ~children) =>
    //   ReasonReact.wrapJsForReason(
    //     ~reactClass=provider(pair),
    //     ~props={"value": value},
    //     children,
    //   );
  };

  module Consumer = {
    let make = (~children: Config.t => React.element) => children;
  };
};
