spartbot-notify
===============

This is a ultra simple Spark Bot submitting a timestamp
to the room you specified.

## How to clone it.

Don't forget to add --recursive option.

    % git clone --recursive https://github.com/tanupoo/sparkbot-notify.git

## How to build it.

You need to add tini module from github.

    % git submodule add https://github.com/tanupoo/ioxutil.git

Then, simpley type make.

    % make

That's it.

## How to run it.

See https://developer.ciscospark.com/index.html

1. Get your access key and room id.
2. Edit sample-test.sh to set your environment variables.
3. Edit package_config.ini properly.
4. Then, execute sample-test.sh.

    % sh sample-test.sh

Enjoy.
