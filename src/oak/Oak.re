module File = File;

let run = (_flags, level, path, use_gitignore) => {
  let began_at = Unix.gettimeofday();
  Logs.debug(m => {
    let level' =
      switch (level) {
      | None => "No-limit"
      | Some(l) => l |> string_of_int
      };
    m(
      {j|
Use GitIgnore: %b
Path: %s
Level: %s|j},
      use_gitignore,
      path,
      level',
    );
  });

  let ignorefile = path |> Git.ignore_file;
  let ignore =
    switch (ignorefile) {
    | Some(file) => file |> Git.should_ignore
    | None => (_ => true)
    };

  Logs.info(m =>
    switch (ignorefile) {
    | None => m("Could not find .ignore file in %s", path)
    | Some(_) => m("Found .ignore file in %s", path)
    }
  );
  Logs.debug(m =>
    switch (ignorefile) {
    | None => ()
    | Some(lines) =>
      m(".gitignore contents: \n%s", String.concat("\n", lines))
    }
  );

  Logs.info(m => m("Listing files starting from path %s...", path));

  let files = path |> File.find_all(~ignore);
  files |> List.iter(File.print);

  Logs.info(m => {
    let delta = Unix.gettimeofday() -. began_at;
    m("Listed %d files in %.5fs", files |> List.length, delta);
  });
};
