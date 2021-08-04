-- insert values
INSERT INTO commands (id, name, platform, os, description, command, examples, tags, refs, datemod, icon)
  VALUES (1, "Create new git branch", "git", "linux", "Create a new branch with git.", "git -b <branch_name>", "git -b new_branch", "stackoverflow.com", "git; version control;", "2021/1/1 9:20:21", "git.img");
INSERT INTO commands (id, name, platform, os, description, command, examples, tags, refs, datemod, icon)
  VALUES (2, "Run python script", "python", "linux", "Run a python3 script.", "python3 -m <script_name.py>", "python3 -m my_script.py", "stackoverflow.com", "research;", "2021/1/1 9:22:21", "python.img");
INSERT INTO commands (id, name, platform, os, description, command, examples, tags, refs, datemod, icon)
  VALUES (3, "Search for help online", "help", "browser", "Search for help online.", "google.com: <search_query> <filters>", "center div site:stackoverflow.com", "stackoverflow.com", "css; web;", "2021/1/1 9:24:21", "google.img");
