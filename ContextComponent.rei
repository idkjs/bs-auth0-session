module MakePair:
  (Config: {
     type t;
     let defaultValue: t;
   }) =>
   {
    type t = Config.t;
    module Provider: {
      let make: (~value: Config.t, ~children: React.element) => React.element;
    };
    module Consumer: {let make: (Config.t => React.element) => React.element;};
  };
