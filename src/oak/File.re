let cwd = ".";

let print = file => Printf.printf("%s\n%!", file);

let (<|>) = (a, b) =>
  switch (a) {
  | Ok(x) => x
  | _ => b
  };

let readfile = path => path |> Fpath.v |> Bos.OS.File.read_lines <|> [];

let exists = path =>
  switch (path |> Unix.stat) {
  | exception _ => false
  | _ => true
  };

let readdir = dir =>
  dir
  |> Sys.readdir
  |> Array.to_list
  |> List.map(Filename.concat(dir))
  |> List.partition(Sys.is_directory);

let rec find_all = (~ignore, dir) => {
  Logs.debug(m => m("Entering directory %s...", dir));

  if (ignore(dir)) {
    [];
  } else {
    switch (dir |> readdir) {
    | exception e =>
      Logs.err(m => m("At path %s: %s", dir, e |> Printexc.to_string));
      [];
    | (dirs, files) =>
      Logs.debug(m => {
        let file_count = files |> List.length;
        let dir_count = dirs |> List.length;
        m("Found %d files and %d subdirectories", file_count, dir_count);
      });

      let this_level = [files, [dir], dirs] |> List.concat;

      let next_level = dirs |> List.map(find_all(~ignore)) |> List.concat;

      [this_level, next_level] |> List.concat;
    };
  };
};
