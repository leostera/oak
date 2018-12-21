module File = {
  open Lwt.Infix;

  let tag = path =>
    Unix.(
      path
      |> Lwt_unix.stat
      >|= (
        stat =>
          switch (stat.st_kind) {
          | S_DIR => `Dir(path)
          | _ => `File(path)
          }
      )
    );

  let cwd = ".";

  let print = file => Printf.printf("%s\n", file);

  let rec find_all = dir =>
    dir
    |> Lwt_unix.files_of_directory
    |> Lwt_stream.map_s(path =>
         path
         |> Filename.concat(dir)
         |> tag
         >|= (
           fun
           | `File(file) => [file] |> Lwt_stream.of_list
           | `Dir(d) =>
             switch (Filename.basename(d)) {
             | "." as d' => [d'] |> Lwt_stream.of_list
             | ".." => [] |> Lwt_stream.of_list
             | _ => find_all(d)
             }
         )
       )
    |> Lwt_stream.concat
    |> Lwt_stream.append(Lwt_stream.of_list([dir]));
};

let run = () =>
  File.cwd |> File.find_all |> Lwt_stream.iter(File.print) |> Lwt_main.run;
