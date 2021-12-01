OUTPUT_FORMAT("binary");

READ_IPL_ADDRESS = 0x7C00;

SECTIONS {
    .       =   READ_IPL_ADDRESS;
    .text   :   {*(.text)}
}
