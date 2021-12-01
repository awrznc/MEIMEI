OUTPUT_FORMAT("binary");

READ_BOOT_ADDRESS = 0xC200;

SECTIONS {
	.       =   READ_BOOT_ADDRESS;
    .text   :   {*(.text)}
}
