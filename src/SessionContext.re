open Session;
module Option = Belt.Option;

type t' = (Session.t, option(Session.errorEvent));
type t = t';

module Impl =
  ContextComponent.MakePair({
    type t = option(t');
    let defaultValue = None;
  });

module Provider = {
  type state = {
    error: option(Session.errorEvent),
    initialised: bool,
    session: Session.t,
  };
  type action =
    | Initialise(Session.t)
    | LoginError(Session.errorEvent)
    | Login(Session.loginEvent)
    | Redirect(string)
    | UpdateSession(Session.t);

  let loginError = (error, dispatch) => LoginError(error) |> dispatch;
  let loginHandler = (event, dispatch) => Login(event) |> dispatch;
  let sessionRenewHandler = (event: Session.loginEvent, dispatch) =>
    UpdateSession(event.session) |> dispatch;

  [@react.component]
  let make =
      (~domain, ~clientId, ~callbackUrl, ~audience=?, ~scope=?, ~children) => {
    let initialState = {
      error: None,
      initialised: false,
      session:
        Session.make(
          ~domain,
          ~clientId,
          ~callbackUrl,
          ~audience?,
          ~scope?,
          (),
        ),
    };

    let (state, dispatch) =
      React.useReducer(
        (state, action) =>
          switch (action) {
          | Initialise(session) =>
            session |> doRenew |> ignore;
            {...state, initialised: true, session};
          | Redirect(url) =>
            url |> ReasonReact.Router.push |> ignore;
            state;
          | UpdateSession(session) => {...state, session, error: None}
          | Login({session, returnUrl}) =>
          Js.log(returnUrl);
          {...state, session, error: None};
            // ReasonReact.SideEffects(
            //   self => {
            //     self.send(
            //       returnUrl |> Option.getWithDefault("/") |> Redirect,
            //     );
            //     self.send(session |> UpdateSession);
            //   },
            // )
          | LoginError(error) =>
            Js.log2("Login error", error);
            let session = state.session |> doLocalLogout;
            let newError =
              error.error == "login_required" ? None : Some(error);
            // ReasonReact.UpdateWithSideEffects(
            //   {...state, session, error: newError},
            //   (self => self.send(Redirect("/login"))),
            // );
            {...state, session, error: newError};
          },
        initialState,
      );
    React.useEffect0(() => {
      switch (state.session){
        
      }
      let session =
        state.session
        |> onLogin(loginHandler(dispatch))
        |> onRenew(dispatch(sessionRenewHandler))
        |> onError(dispatch(loginError));

      Some(() => Initialise(session));
    });

    // render: ({state: {initialised, session, error}}) =>
    <Impl.Provider value={state.initialised ? Some((state.session, state.error)) : None}>
      children
    </Impl.Provider>;
  };
};

module Consumer = {
  [@react.component]
  let make = (~children) => {
    <Impl.Consumer>
      ...{
           fun
           | Some(session) => children(session)
           | None => React.null
         }
    </Impl.Consumer>;
  };
};

module Manager = {
  [@react.component]
  let make =
      (~domain, ~clientId, ~callbackUrl, ~audience=?, ~scope=?, ~children) => {
    <Provider domain clientId callbackUrl ?audience ?scope>
      <Consumer> children </Consumer>
    </Provider>;
  };
};

module ErrorConsumer = {
  [@react.component]
  let make = (~children) => {
    <Impl.Consumer>
      ...{
           fun
           | Some((_, error)) => children(error)
           | None => React.null
         }
    </Impl.Consumer>;
  };
};

module LoggedOutConsumer = {
  [@react.component]
  let make = (~children) => {
    <Impl.Consumer>
      ...{session =>
        switch (
          session->Option.map(s => s->fst),
          session
          ->Option.map(s => s->fst->isLoggedIn)
          ->Option.getWithDefault(false),
        ) {
        | (_, true) => React.null
        | (None, _) => children(true)
        | (Some(session), _) => children(session->isPending)
        }
      }
    </Impl.Consumer>;
  };
};

module LoggedInConsumer = {
  [@react.component]
  let make = (~children) => {
    <Impl.Consumer>
      ...{session =>
        switch (
          session->Option.map(s => s->fst),
          session
          ->Option.map(s => s->fst->isLoggedIn)
          ->Option.getWithDefault(false),
        ) {
        | (Some(session), true) => children(session)
        | _ => React.null
        }
      }
    </Impl.Consumer>;
  };
};

module IdConsumer = {
  [@react.component]
  let make = (~children) => {
    <Impl.Consumer>
      ...{session =>
        switch (session->Option.flatMap(s => s->fst->idGet)) {
        | Some(id) => children(id)
        | None => React.null
        }
      }
    </Impl.Consumer>;
  };
};
