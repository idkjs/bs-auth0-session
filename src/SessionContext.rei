type t = (Session.t, option(Session.errorEvent));

/**
 * Sets up the provider for the Session. You need one of these near the root of your app.
 */
module Provider: {
  type state;
  type action;
  let make:
    (
      ~domain: string,
      ~clientId: string,
      ~callbackUrl: string,
      ~audience: string=?,
      ~scope: string=?,
      ~children: React.element
    ) =>
    React.element;
};

/**
 * This is a consumer that exposes the raw session data.
 *
 * This is mainly intended as an escape hatch if the specific data you want isn't exposed by a specialised consumer.
 */
module Consumer: {let make: (t => React.element) => React.element;};

/**
 * Provider + Consumer, wrapped up in one easy-to-use component.
 */
module Manager: {
  let make:
    (
      ~domain: string,
      ~clientId: string,
      ~callbackUrl: string,
      ~audience: string=?,
      ~scope: string=?,
      t => React.element
    ) =>
    React.element;
};

/**
 * Only exposes error data. Useful for displaying error messages.
 */
module ErrorConsumer: {
  let make: (option(Session.errorEvent) => React.element) => React.element;
};

/**
 * Only renders its children if the user session is currently not logged in.
 *
 * The boolean provided to the children function is whether the session is still "pending" and hasn't been
 * verified as either logged in or logged out with Auth0 yet.
 */
module LoggedOutConsumer: {let make: (bool => React.element) => React.element;
};

/**
 * Only renders its children if the user session is currently logged in.
 *
 * Exposes the full session to its children.
 */
module LoggedInConsumer: {
  let make: (Session.t => React.element) => React.element;
};

/**
 * Only render its children if the user session is currently logged in.
 *
 * Exposes the id_token data directly instead of the entire session.
 */
module IdConsumer: {let make: (Session.id => React.element) => React.element;};
