 export TSUKI_CONFIG_PATH="${PWD}/examples/config/init.lua" 
 export TSUKI_LIB_PATH="${PWD}/lib" 

echo "lib path: ${TSUKI_LIB_PATH}"


 meson compile -C build

./build/src/tsuki
