module File = {
  let cwd = ".";

  let print = file => Printf.printf("%s\n", file);

  let rec find_all: string => list(string) =
    dir => {
      let (dirs, files) =
        dir
        |> Sys.readdir
        |> Array.to_list
        |> List.map(Filename.concat(dir))
        |> List.partition(Sys.is_directory);

      let this_level = [files, [dir], dirs] |> List.concat;

      let next_level = dirs |> List.map(find_all) |> List.concat;

      [this_level, next_level] |> List.concat;
    };
};

let run = () => File.cwd |> File.find_all |> List.map(File.print);
