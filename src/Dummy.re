module DummyContext = {
  let context = React.createContext(0);
  module Provider = {
    include React.Context;
    let make = context->React.Context.provider;
  };

  module Consumer = {
    [@react.component]
    let make = () => {
      let value = React.useContext(context);
      <div> value->React.int </div>;
    };
  };
};
