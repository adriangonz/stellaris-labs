
function get_filename() {
    file=`basename $1`
    filename=${file%.*}
    echo $filename
}

function build_arm() {
    
    PATH_TO_SDK=/Users/kaseyo/Development/Stuff/stellarisware
    STARTUP_CODE=startup_gcc.c
    LD_SCRIPT=linker.ld
    PATH_TO_EXAMPLE=$PATH_TO_SDK/boards/ek-lm4f120xl/blinky
    PATH_TO_LD=$PATH_TO_EXAMPLE/blinky.ld
    PATH_TO_STARTUP=$PATH_TO_EXAMPLE/$STARTUP_CODE
    
    # Copy linker script and startup code to current dir
    cp $PATH_TO_STARTUP ./$STARTUP_CODE
    cp $PATH_TO_LD ./$LD_SCRIPT

    # Compile *.c
    echo "[ARM Build] Compiling *.c files..."
    arm-none-eabi-gcc $1 $STARTUP_CODE -g \
        -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
        -mfloat-abi=softfp -Os -ffunction-sections \
        -fdata-sections -MD -std=c99 -Wall -pedantic \
        -DPART_LM4F120H5QR -c -I$PATH_TO_SDK \
        -DTARGET_IS_BLIZZARD_RA1

    # Get .o filenames
    STARTUP_OBJ="`get_filename $STARTUP_CODE`.o"
    MAIN_OBJ="`get_filename $1`.o"

    # Link
    echo "[ARM Build] Linking objects..."
    arm-none-eabi-ld -T $LD_SCRIPT --entry ResetISR -o a.out $STARTUP_OBJ $MAIN_OBJ

    # Create binary
    echo "[ARM Build] Creating binary..."
    arm-none-eabi-objcopy -O binary a.out "`get_filename $1`.bin"
}

if [ "$1" = "" ]
then
    echo "[ARM Build] Usage: ./build.sh MAIN"
else
    build_arm "$1"
    if [[ $? -eq 0 ]]
    then
        echo "[ARM Build] Binary generated at `get_filename $1`.bin!"
        echo "[ARM Build] Execute \"lm4flash `get_filename $1`.bin\" to load it in the board."
        echo "[ARM Build] Happy hacking :)"
    fi
fi
