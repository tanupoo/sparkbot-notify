spartbot-notify
===============

This is a ultra simple Spark Bot submitting a timestamp
to the room you specified.

## How to build it.

Don't forget to add --recursive option when you clone it.

    % git clone --recursive https://github.com/tanupoo/sparkbot-notify.git

It uses *libcurl* to submit the messages.
Typical OS has the library from the initial.
If not, you need to install it first.

Then, simpley type make.

    % cd sparkbot-notify
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
