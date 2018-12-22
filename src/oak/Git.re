let gitdir = "./.git";
let gitignore = "./.gitignore";

let ignore_file = path => {
  let fullpath = Filename.concat(path, gitignore);
  if (fullpath |> File.exists) {
    let contents = fullpath |> File.readfile;
    Some(contents);
  } else {
    None;
  };
};

let should_ignore = (paths, path) => {
  let result = List.mem(path, [gitdir, gitignore, ...paths]);
  Logs.debug(m => result ? m("Ignoring path: %s", path) : ());
  result;
};
