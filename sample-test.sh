#!/bin/sh

export CAF_APP_PATH="./"
export CAF_APP_CONFIG_FILE="package_config.ini"

export CAF_APP_PERSISTENT_DIR="/data"
export CAF_APP_LOG_DIR="/log"
export CAF_APP_CONFIG_DIR="/config"
export CAF_APP_USERNAME="username"
export CAF_HOME="caf"
export CAF_HOME_ABS_PATH="/home/caf"
export CAF_MODULES_PATH="/module_path"
export CAF_APP_DIR="/app_dir"
export CAF_MODULES_DIR="/module_dir"
export CAF_APP_ID="1111"

./sparkbot-notify -dd
