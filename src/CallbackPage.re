[@react.component]
let make = () => {
  <SessionContext.Consumer>
    ...{((session, _)) => {
      session->Session.doCallback;
      React.null;
    }}
  </SessionContext.Consumer>;
};
