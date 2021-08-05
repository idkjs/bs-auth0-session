[@react.component]
let make = (~returnUrl) => {
  <SessionContext.Consumer>
    ...{((session, _)) => {
      session->Session.doLogout(~returnUrl, ());
      React.null;
    }}
  </SessionContext.Consumer>;
};
