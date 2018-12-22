open Cmdliner;

let setup_verbosity = (level, debug) => {
  Fmt_tty.setup_std_outputs();
  if (debug) {
    Logs.set_level(Some(Logs.Debug));
  } else {
    Logs.set_level(level);
  };
  Logs.set_reporter(Logs_fmt.reporter());
};

module Args = {
  let debug = {
    let doc = "Shortcut for debugging verbosity";
    Arg.(value & flag & info(["d", "debug"], ~doc));
  };

  let verbosity = Term.(const(setup_verbosity) $ Logs_cli.level() $ debug);

  let use_gitignore = {
    let doc = "Whether to ignore files in .gitignore";
    Arg.(value & flag & info(["use-gitignore"], ~doc));
  };

  let level = {
    let doc = "How many levels deep to go";
    Arg.(
      value
      & opt(some(int), None)
      & info(["l", "level"], ~docv="LEVEL", ~doc)
    );
  };

  let path = {
    let doc = "Path from where to start listing files";
    Arg.(
      value & pos(0, string, Oak.File.cwd) & info([], ~docv="PATH", ~doc)
    );
  };
};

let cmd =
  Term.(
    const(Oak.run)
    $ Args.verbosity
    $ Args.level
    $ Args.path
    $ Args.use_gitignore
  );

Term.eval((cmd, Term.info("oak"))) |> Term.exit;
