#include <stddef.h>

#include "unichar_decomposition_data.h"
#include "nds_extension_macros.h"

static const unichar DT_0000[] =
{
    1 /*character count*/,
    0x0020 /*SPACE*/,
};
static const unichar DT_0001[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0002[] =
{
    1 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
};
static const unichar DT_0003[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0004[] =
{
    1 /*character count*/,
    0x0032 /*DIGIT TWO*/,
};
static const unichar DT_0005[] =
{
    1 /*character count*/,
    0x0033 /*DIGIT THREE*/,
};
static const unichar DT_0006[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0007[] =
{
    1 /*character count*/,
    0x03bc /*GREEK SMALL LETTER MU*/,
};
static const unichar DT_0008[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0009[] =
{
    1 /*character count*/,
    0x0031 /*DIGIT ONE*/,
};
static const unichar DT_0010[] =
{
    1 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
};
static const unichar DT_0011[] =
{
    3 /*character count*/,
    0x0031 /*DIGIT ONE*/,
    0x2044 /*FRACTION SLASH*/,
    0x0034 /*DIGIT FOUR*/,
};
static const unichar DT_0012[] =
{
    3 /*character count*/,
    0x0031 /*DIGIT ONE*/,
    0x2044 /*FRACTION SLASH*/,
    0x0032 /*DIGIT TWO*/,
};
static const unichar DT_0013[] =
{
    3 /*character count*/,
    0x0033 /*DIGIT THREE*/,
    0x2044 /*FRACTION SLASH*/,
    0x0034 /*DIGIT FOUR*/,
};
static const unichar DT_0014[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0015[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0016[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0017[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0018[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0019[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x030a /*COMBINING RING ABOVE*/,
};
static const unichar DT_0020[] =
{
    2 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0021[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0022[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0023[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0024[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0025[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0026[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0027[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0028[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0029[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0030[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0031[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0032[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0033[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0034[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0035[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0036[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0037[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0038[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0039[] =
{
    2 /*character count*/,
    0x0059 /*LATIN CAPITAL LETTER Y*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0040[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0041[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0042[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0043[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0044[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0045[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x030a /*COMBINING RING ABOVE*/,
};
static const unichar DT_0046[] =
{
    2 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0047[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0048[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0049[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0050[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0051[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0052[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0053[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0054[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0055[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0056[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0057[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0058[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0059[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0060[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0061[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0062[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0063[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0064[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0065[] =
{
    2 /*character count*/,
    0x0079 /*LATIN SMALL LETTER Y*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0066[] =
{
    2 /*character count*/,
    0x0079 /*LATIN SMALL LETTER Y*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0067[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0068[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0069[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0070[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0071[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0072[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0073[] =
{
    2 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0074[] =
{
    2 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0075[] =
{
    2 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0076[] =
{
    2 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0077[] =
{
    2 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0078[] =
{
    2 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0079[] =
{
    2 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0080[] =
{
    2 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0081[] =
{
    2 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0082[] =
{
    2 /*character count*/,
    0x0064 /*LATIN SMALL LETTER D*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0083[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0084[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0085[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0086[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0087[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0088[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0089[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0090[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0091[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0092[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0093[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0094[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0095[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0096[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0097[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0098[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0099[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0100[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0101[] =
{
    2 /*character count*/,
    0x0048 /*LATIN CAPITAL LETTER H*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0102[] =
{
    2 /*character count*/,
    0x0068 /*LATIN SMALL LETTER H*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0103[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0104[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0105[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0106[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0107[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0108[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0109[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0110[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0111[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0112[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x004a /*LATIN CAPITAL LETTER J*/,
};
static const unichar DT_0113[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0114[] =
{
    2 /*character count*/,
    0x004a /*LATIN CAPITAL LETTER J*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0115[] =
{
    2 /*character count*/,
    0x006a /*LATIN SMALL LETTER J*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0116[] =
{
    2 /*character count*/,
    0x004b /*LATIN CAPITAL LETTER K*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0117[] =
{
    2 /*character count*/,
    0x006b /*LATIN SMALL LETTER K*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0118[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0119[] =
{
    2 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0120[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0121[] =
{
    2 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0122[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0123[] =
{
    2 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0124[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x00b7 /*MIDDLE DOT*/,
};
static const unichar DT_0125[] =
{
    2 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
    0x00b7 /*MIDDLE DOT*/,
};
static const unichar DT_0126[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0127[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0128[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0129[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0130[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0131[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0132[] =
{
    2 /*character count*/,
    0x02bc /*MODIFIER LETTER APOSTROPHE*/,
    0x006e /*LATIN SMALL LETTER N*/,
};
static const unichar DT_0133[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0134[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0135[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0136[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0137[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x030b /*COMBINING DOUBLE ACUTE ACCENT*/,
};
static const unichar DT_0138[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x030b /*COMBINING DOUBLE ACUTE ACCENT*/,
};
static const unichar DT_0139[] =
{
    2 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0140[] =
{
    2 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0141[] =
{
    2 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0142[] =
{
    2 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0143[] =
{
    2 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0144[] =
{
    2 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0145[] =
{
    2 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0146[] =
{
    2 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0147[] =
{
    2 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0148[] =
{
    2 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0149[] =
{
    2 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0150[] =
{
    2 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0151[] =
{
    2 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0152[] =
{
    2 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0153[] =
{
    2 /*character count*/,
    0x0054 /*LATIN CAPITAL LETTER T*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0154[] =
{
    2 /*character count*/,
    0x0074 /*LATIN SMALL LETTER T*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0155[] =
{
    2 /*character count*/,
    0x0054 /*LATIN CAPITAL LETTER T*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0156[] =
{
    2 /*character count*/,
    0x0074 /*LATIN SMALL LETTER T*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0157[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0158[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0303 /*COMBINING TILDE*/,
};
static const unichar DT_0159[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0160[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0161[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0162[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0306 /*COMBINING BREVE*/,
};
static const unichar DT_0163[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x030a /*COMBINING RING ABOVE*/,
};
static const unichar DT_0164[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x030a /*COMBINING RING ABOVE*/,
};
static const unichar DT_0165[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x030b /*COMBINING DOUBLE ACUTE ACCENT*/,
};
static const unichar DT_0166[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x030b /*COMBINING DOUBLE ACUTE ACCENT*/,
};
static const unichar DT_0167[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0168[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0169[] =
{
    2 /*character count*/,
    0x0057 /*LATIN CAPITAL LETTER W*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0170[] =
{
    2 /*character count*/,
    0x0077 /*LATIN SMALL LETTER W*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0171[] =
{
    2 /*character count*/,
    0x0059 /*LATIN CAPITAL LETTER Y*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0172[] =
{
    2 /*character count*/,
    0x0079 /*LATIN SMALL LETTER Y*/,
    0x0302 /*COMBINING CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0173[] =
{
    2 /*character count*/,
    0x0059 /*LATIN CAPITAL LETTER Y*/,
    0x0308 /*COMBINING DIAERESIS*/,
};
static const unichar DT_0174[] =
{
    2 /*character count*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0175[] =
{
    2 /*character count*/,
    0x007a /*LATIN SMALL LETTER Z*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0176[] =
{
    2 /*character count*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0177[] =
{
    2 /*character count*/,
    0x007a /*LATIN SMALL LETTER Z*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0178[] =
{
    2 /*character count*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0179[] =
{
    2 /*character count*/,
    0x007a /*LATIN SMALL LETTER Z*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0180[] =
{
    1 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
};
static const unichar DT_0181[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x031b /*COMBINING HORN*/,
};
static const unichar DT_0182[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x031b /*COMBINING HORN*/,
};
static const unichar DT_0183[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x031b /*COMBINING HORN*/,
};
static const unichar DT_0184[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x031b /*COMBINING HORN*/,
};
static const unichar DT_0185[] =
{
    3 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0186[] =
{
    3 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
    0x007a /*LATIN SMALL LETTER Z*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0187[] =
{
    3 /*character count*/,
    0x0064 /*LATIN SMALL LETTER D*/,
    0x007a /*LATIN SMALL LETTER Z*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0188[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x004a /*LATIN CAPITAL LETTER J*/,
};
static const unichar DT_0189[] =
{
    2 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0190[] =
{
    2 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0191[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x004a /*LATIN CAPITAL LETTER J*/,
};
static const unichar DT_0192[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0193[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0194[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0195[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0196[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0197[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0198[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0199[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0200[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0201[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0202[] =
{
    3 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0203[] =
{
    3 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0204[] =
{
    3 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0205[] =
{
    3 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0206[] =
{
    3 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0207[] =
{
    3 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0208[] =
{
    3 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0209[] =
{
    3 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0210[] =
{
    3 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0211[] =
{
    3 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0212[] =
{
    3 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0307 /*COMBINING DOT ABOVE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0213[] =
{
    3 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0307 /*COMBINING DOT ABOVE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0214[] =
{
    2 /*character count*/,
    0x00c6 /*LATIN CAPITAL LETTER AE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0215[] =
{
    2 /*character count*/,
    0x00e6 /*LATIN SMALL LETTER AE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0216[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0217[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0218[] =
{
    2 /*character count*/,
    0x004b /*LATIN CAPITAL LETTER K*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0219[] =
{
    2 /*character count*/,
    0x006b /*LATIN SMALL LETTER K*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0220[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0221[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0328 /*COMBINING OGONEK*/,
};
static const unichar DT_0222[] =
{
    3 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0328 /*COMBINING OGONEK*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0223[] =
{
    3 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0328 /*COMBINING OGONEK*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0224[] =
{
    2 /*character count*/,
    0x01b7 /*LATIN CAPITAL LETTER EZH*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0225[] =
{
    2 /*character count*/,
    0x0292 /*LATIN SMALL LETTER EZH*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0226[] =
{
    2 /*character count*/,
    0x006a /*LATIN SMALL LETTER J*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0227[] =
{
    2 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
};
static const unichar DT_0228[] =
{
    2 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
    0x007a /*LATIN SMALL LETTER Z*/,
};
static const unichar DT_0229[] =
{
    2 /*character count*/,
    0x0064 /*LATIN SMALL LETTER D*/,
    0x007a /*LATIN SMALL LETTER Z*/,
};
static const unichar DT_0230[] =
{
    2 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0231[] =
{
    2 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0232[] =
{
    2 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0233[] =
{
    2 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
    0x0300 /*COMBINING GRAVE ACCENT*/,
};
static const unichar DT_0234[] =
{
    3 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x030a /*COMBINING RING ABOVE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0235[] =
{
    3 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x030a /*COMBINING RING ABOVE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0236[] =
{
    2 /*character count*/,
    0x00c6 /*LATIN CAPITAL LETTER AE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0237[] =
{
    2 /*character count*/,
    0x00e6 /*LATIN SMALL LETTER AE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0238[] =
{
    2 /*character count*/,
    0x00d8 /*LATIN CAPITAL LETTER O WITH STROKE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0239[] =
{
    2 /*character count*/,
    0x00f8 /*LATIN SMALL LETTER O WITH STROKE*/,
    0x0301 /*COMBINING ACUTE ACCENT*/,
};
static const unichar DT_0240[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0241[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0242[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0243[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0244[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0245[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0246[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0247[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0248[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0249[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0250[] =
{
    2 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0251[] =
{
    2 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0252[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0253[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0254[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0255[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0256[] =
{
    2 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0257[] =
{
    2 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0258[] =
{
    2 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0259[] =
{
    2 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0260[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0261[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x030f /*COMBINING DOUBLE GRAVE ACCENT*/,
};
static const unichar DT_0262[] =
{
    2 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0263[] =
{
    2 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
    0x0311 /*COMBINING INVERTED BREVE*/,
};
static const unichar DT_0264[] =
{
    2 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
    0x0326 /*COMBINING COMMA BELOW*/,
};
static const unichar DT_0265[] =
{
    2 /*character count*/,
    0x0073 /*LATIN SMALL LETTER S*/,
    0x0326 /*COMBINING COMMA BELOW*/,
};
static const unichar DT_0266[] =
{
    2 /*character count*/,
    0x0054 /*LATIN CAPITAL LETTER T*/,
    0x0326 /*COMBINING COMMA BELOW*/,
};
static const unichar DT_0267[] =
{
    2 /*character count*/,
    0x0074 /*LATIN SMALL LETTER T*/,
    0x0326 /*COMBINING COMMA BELOW*/,
};
static const unichar DT_0268[] =
{
    2 /*character count*/,
    0x0048 /*LATIN CAPITAL LETTER H*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0269[] =
{
    2 /*character count*/,
    0x0068 /*LATIN SMALL LETTER H*/,
    0x030c /*COMBINING CARON*/,
};
static const unichar DT_0270[] =
{
    2 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0271[] =
{
    2 /*character count*/,
    0x0061 /*LATIN SMALL LETTER A*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0272[] =
{
    2 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0273[] =
{
    2 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
    0x0327 /*COMBINING CEDILLA*/,
};
static const unichar DT_0274[] =
{
    3 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0275[] =
{
    3 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0308 /*COMBINING DIAERESIS*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0276[] =
{
    3 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0303 /*COMBINING TILDE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0277[] =
{
    3 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0303 /*COMBINING TILDE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0278[] =
{
    2 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0279[] =
{
    2 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0307 /*COMBINING DOT ABOVE*/,
};
static const unichar DT_0280[] =
{
    3 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
    0x0307 /*COMBINING DOT ABOVE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0281[] =
{
    3 /*character count*/,
    0x006f /*LATIN SMALL LETTER O*/,
    0x0307 /*COMBINING DOT ABOVE*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0282[] =
{
    2 /*character count*/,
    0x0059 /*LATIN CAPITAL LETTER Y*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0283[] =
{
    2 /*character count*/,
    0x0079 /*LATIN SMALL LETTER Y*/,
    0x0304 /*COMBINING MACRON*/,
};
static const unichar DT_0284[] =
{
    1 /*character count*/,
    0x3042 /*HIRAGANA LETTER A*/,
};
static const unichar DT_0285[] =
{
    1 /*character count*/,
    0x3044 /*HIRAGANA LETTER I*/,
};
static const unichar DT_0286[] =
{
    1 /*character count*/,
    0x3046 /*HIRAGANA LETTER U*/,
};
static const unichar DT_0287[] =
{
    1 /*character count*/,
    0x3048 /*HIRAGANA LETTER E*/,
};
static const unichar DT_0288[] =
{
    1 /*character count*/,
    0x304a /*HIRAGANA LETTER O*/,
};
static const unichar DT_0289[] =
{
    2 /*character count*/,
    0x304b /*HIRAGANA LETTER KA*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0290[] =
{
    2 /*character count*/,
    0x304d /*HIRAGANA LETTER KI*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0291[] =
{
    2 /*character count*/,
    0x304f /*HIRAGANA LETTER KU*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0292[] =
{
    2 /*character count*/,
    0x3051 /*HIRAGANA LETTER KE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0293[] =
{
    2 /*character count*/,
    0x3053 /*HIRAGANA LETTER KO*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0294[] =
{
    2 /*character count*/,
    0x3055 /*HIRAGANA LETTER SA*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0295[] =
{
    2 /*character count*/,
    0x3057 /*HIRAGANA LETTER SI*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0296[] =
{
    2 /*character count*/,
    0x3059 /*HIRAGANA LETTER SU*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0297[] =
{
    2 /*character count*/,
    0x305b /*HIRAGANA LETTER SE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0298[] =
{
    2 /*character count*/,
    0x305d /*HIRAGANA LETTER SO*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0299[] =
{
    2 /*character count*/,
    0x305f /*HIRAGANA LETTER TA*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0300[] =
{
    2 /*character count*/,
    0x3061 /*HIRAGANA LETTER TI*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0301[] =
{
    1 /*character count*/,
    0x3064 /*HIRAGANA LETTER TU*/,
};
static const unichar DT_0302[] =
{
    2 /*character count*/,
    0x3064 /*HIRAGANA LETTER TU*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0303[] =
{
    2 /*character count*/,
    0x3066 /*HIRAGANA LETTER TE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0304[] =
{
    2 /*character count*/,
    0x3068 /*HIRAGANA LETTER TO*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0305[] =
{
    2 /*character count*/,
    0x306f /*HIRAGANA LETTER HA*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0306[] =
{
    2 /*character count*/,
    0x306f /*HIRAGANA LETTER HA*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0307[] =
{
    2 /*character count*/,
    0x3072 /*HIRAGANA LETTER HI*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0308[] =
{
    2 /*character count*/,
    0x3072 /*HIRAGANA LETTER HI*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0309[] =
{
    2 /*character count*/,
    0x3075 /*HIRAGANA LETTER HU*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0310[] =
{
    2 /*character count*/,
    0x3075 /*HIRAGANA LETTER HU*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0311[] =
{
    2 /*character count*/,
    0x3078 /*HIRAGANA LETTER HE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0312[] =
{
    2 /*character count*/,
    0x3078 /*HIRAGANA LETTER HE*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0313[] =
{
    2 /*character count*/,
    0x307b /*HIRAGANA LETTER HO*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0314[] =
{
    2 /*character count*/,
    0x307b /*HIRAGANA LETTER HO*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0315[] =
{
    1 /*character count*/,
    0x3084 /*HIRAGANA LETTER YA*/,
};
static const unichar DT_0316[] =
{
    1 /*character count*/,
    0x3086 /*HIRAGANA LETTER YU*/,
};
static const unichar DT_0317[] =
{
    1 /*character count*/,
    0x3088 /*HIRAGANA LETTER YO*/,
};
static const unichar DT_0318[] =
{
    1 /*character count*/,
    0x308f /*HIRAGANA LETTER WA*/,
};
static const unichar DT_0319[] =
{
    2 /*character count*/,
    0x3046 /*HIRAGANA LETTER U*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0320[] =
{
    1 /*character count*/,
    0x304b /*HIRAGANA LETTER KA*/,
};
static const unichar DT_0321[] =
{
    1 /*character count*/,
    0x3051 /*HIRAGANA LETTER KE*/,
};
static const unichar DT_0322[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0323[] =
{
    2 /*character count*/,
    0x0020 /*SPACE*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};
static const unichar DT_0324[] =
{
    2 /*character count*/,
    0x309d /*HIRAGANA ITERATION MARK*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0325[] =
{
    2 /*character count*/,
    0x3088 /*HIRAGANA LETTER YO*/,
    0x308a /*HIRAGANA LETTER RI*/,
};
static const unichar DT_0326[] =
{
    1 /*character count*/,
    0x304d /*HIRAGANA LETTER KI*/,
};
static const unichar DT_0327[] =
{
    1 /*character count*/,
    0x304f /*HIRAGANA LETTER KU*/,
};
static const unichar DT_0328[] =
{
    1 /*character count*/,
    0x3053 /*HIRAGANA LETTER KO*/,
};
static const unichar DT_0329[] =
{
    1 /*character count*/,
    0x3055 /*HIRAGANA LETTER SA*/,
};
static const unichar DT_0330[] =
{
    1 /*character count*/,
    0x3057 /*HIRAGANA LETTER SI*/,
};
static const unichar DT_0331[] =
{
    1 /*character count*/,
    0x3059 /*HIRAGANA LETTER SU*/,
};
static const unichar DT_0332[] =
{
    1 /*character count*/,
    0x305b /*HIRAGANA LETTER SE*/,
};
static const unichar DT_0333[] =
{
    1 /*character count*/,
    0x305d /*HIRAGANA LETTER SO*/,
};
static const unichar DT_0334[] =
{
    1 /*character count*/,
    0x305f /*HIRAGANA LETTER TA*/,
};
static const unichar DT_0335[] =
{
    1 /*character count*/,
    0x3061 /*HIRAGANA LETTER TI*/,
};
static const unichar DT_0336[] =
{
    1 /*character count*/,
    0x3066 /*HIRAGANA LETTER TE*/,
};
static const unichar DT_0337[] =
{
    1 /*character count*/,
    0x3068 /*HIRAGANA LETTER TO*/,
};
static const unichar DT_0338[] =
{
    1 /*character count*/,
    0x306a /*HIRAGANA LETTER NA*/,
};
static const unichar DT_0339[] =
{
    1 /*character count*/,
    0x306b /*HIRAGANA LETTER NI*/,
};
static const unichar DT_0340[] =
{
    1 /*character count*/,
    0x306c /*HIRAGANA LETTER NU*/,
};
static const unichar DT_0341[] =
{
    1 /*character count*/,
    0x306d /*HIRAGANA LETTER NE*/,
};
static const unichar DT_0342[] =
{
    1 /*character count*/,
    0x306e /*HIRAGANA LETTER NO*/,
};
static const unichar DT_0343[] =
{
    1 /*character count*/,
    0x306f /*HIRAGANA LETTER HA*/,
};
static const unichar DT_0344[] =
{
    1 /*character count*/,
    0x3072 /*HIRAGANA LETTER HI*/,
};
static const unichar DT_0345[] =
{
    1 /*character count*/,
    0x3075 /*HIRAGANA LETTER HU*/,
};
static const unichar DT_0346[] =
{
    1 /*character count*/,
    0x3078 /*HIRAGANA LETTER HE*/,
};
static const unichar DT_0347[] =
{
    1 /*character count*/,
    0x307b /*HIRAGANA LETTER HO*/,
};
static const unichar DT_0348[] =
{
    1 /*character count*/,
    0x307e /*HIRAGANA LETTER MA*/,
};
static const unichar DT_0349[] =
{
    1 /*character count*/,
    0x307f /*HIRAGANA LETTER MI*/,
};
static const unichar DT_0350[] =
{
    1 /*character count*/,
    0x3080 /*HIRAGANA LETTER MU*/,
};
static const unichar DT_0351[] =
{
    1 /*character count*/,
    0x3081 /*HIRAGANA LETTER ME*/,
};
static const unichar DT_0352[] =
{
    1 /*character count*/,
    0x3082 /*HIRAGANA LETTER MO*/,
};
static const unichar DT_0353[] =
{
    1 /*character count*/,
    0x3089 /*HIRAGANA LETTER RA*/,
};
static const unichar DT_0354[] =
{
    1 /*character count*/,
    0x308a /*HIRAGANA LETTER RI*/,
};
static const unichar DT_0355[] =
{
    1 /*character count*/,
    0x308b /*HIRAGANA LETTER RU*/,
};
static const unichar DT_0356[] =
{
    1 /*character count*/,
    0x308c /*HIRAGANA LETTER RE*/,
};
static const unichar DT_0357[] =
{
    1 /*character count*/,
    0x308d /*HIRAGANA LETTER RO*/,
};
static const unichar DT_0358[] =
{
    1 /*character count*/,
    0x3090 /*HIRAGANA LETTER WI*/,
};
static const unichar DT_0359[] =
{
    1 /*character count*/,
    0x3091 /*HIRAGANA LETTER WE*/,
};
static const unichar DT_0360[] =
{
    1 /*character count*/,
    0x3092 /*HIRAGANA LETTER WO*/,
};
static const unichar DT_0361[] =
{
    1 /*character count*/,
    0x3093 /*HIRAGANA LETTER N*/,
};
static const unichar DT_0362[] =
{
    2 /*character count*/,
    0x30ef /*KATAKANA LETTER WA*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0363[] =
{
    2 /*character count*/,
    0x30f0 /*KATAKANA LETTER WI*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0364[] =
{
    2 /*character count*/,
    0x30f1 /*KATAKANA LETTER WE*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0365[] =
{
    2 /*character count*/,
    0x30f2 /*KATAKANA LETTER WO*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0366[] =
{
    1 /*character count*/,
    0x309d /*HIRAGANA ITERATION MARK*/,
};
static const unichar DT_0367[] =
{
    2 /*character count*/,
    0x30b3 /*KATAKANA LETTER KO*/,
    0x30c8 /*KATAKANA LETTER TO*/,
};
static const unichar DT_0368[] =
{
    1 /*character count*/,
    0x0021 /*EXCLAMATION MARK*/,
};
static const unichar DT_0369[] =
{
    1 /*character count*/,
    0x0022 /*QUOTATION MARK*/,
};
static const unichar DT_0370[] =
{
    1 /*character count*/,
    0x0023 /*NUMBER SIGN*/,
};
static const unichar DT_0371[] =
{
    1 /*character count*/,
    0x0024 /*DOLLAR SIGN*/,
};
static const unichar DT_0372[] =
{
    1 /*character count*/,
    0x0025 /*PERCENT SIGN*/,
};
static const unichar DT_0373[] =
{
    1 /*character count*/,
    0x0026 /*AMPERSAND*/,
};
static const unichar DT_0374[] =
{
    1 /*character count*/,
    0x0027 /*APOSTROPHE*/,
};
static const unichar DT_0375[] =
{
    1 /*character count*/,
    0x0028 /*LEFT PARENTHESIS*/,
};
static const unichar DT_0376[] =
{
    1 /*character count*/,
    0x0029 /*RIGHT PARENTHESIS*/,
};
static const unichar DT_0377[] =
{
    1 /*character count*/,
    0x002a /*ASTERISK*/,
};
static const unichar DT_0378[] =
{
    1 /*character count*/,
    0x002b /*PLUS SIGN*/,
};
static const unichar DT_0379[] =
{
    1 /*character count*/,
    0x002c /*COMMA*/,
};
static const unichar DT_0380[] =
{
    1 /*character count*/,
    0x002d /*HYPHEN-MINUS*/,
};
static const unichar DT_0381[] =
{
    1 /*character count*/,
    0x002e /*FULL STOP*/,
};
static const unichar DT_0382[] =
{
    1 /*character count*/,
    0x002f /*SOLIDUS*/,
};
static const unichar DT_0383[] =
{
    1 /*character count*/,
    0x0030 /*DIGIT ZERO*/,
};
static const unichar DT_0384[] =
{
    1 /*character count*/,
    0x0034 /*DIGIT FOUR*/,
};
static const unichar DT_0385[] =
{
    1 /*character count*/,
    0x0035 /*DIGIT FIVE*/,
};
static const unichar DT_0386[] =
{
    1 /*character count*/,
    0x0036 /*DIGIT SIX*/,
};
static const unichar DT_0387[] =
{
    1 /*character count*/,
    0x0037 /*DIGIT SEVEN*/,
};
static const unichar DT_0388[] =
{
    1 /*character count*/,
    0x0038 /*DIGIT EIGHT*/,
};
static const unichar DT_0389[] =
{
    1 /*character count*/,
    0x0039 /*DIGIT NINE*/,
};
static const unichar DT_0390[] =
{
    1 /*character count*/,
    0x003a /*COLON*/,
};
static const unichar DT_0391[] =
{
    1 /*character count*/,
    0x003b /*SEMICOLON*/,
};
static const unichar DT_0392[] =
{
    1 /*character count*/,
    0x003c /*LESS-THAN SIGN*/,
};
static const unichar DT_0393[] =
{
    1 /*character count*/,
    0x003d /*EQUALS SIGN*/,
};
static const unichar DT_0394[] =
{
    1 /*character count*/,
    0x003e /*GREATER-THAN SIGN*/,
};
static const unichar DT_0395[] =
{
    1 /*character count*/,
    0x003f /*QUESTION MARK*/,
};
static const unichar DT_0396[] =
{
    1 /*character count*/,
    0x0040 /*COMMERCIAL AT*/,
};
static const unichar DT_0397[] =
{
    1 /*character count*/,
    0x0041 /*LATIN CAPITAL LETTER A*/,
};
static const unichar DT_0398[] =
{
    1 /*character count*/,
    0x0042 /*LATIN CAPITAL LETTER B*/,
};
static const unichar DT_0399[] =
{
    1 /*character count*/,
    0x0043 /*LATIN CAPITAL LETTER C*/,
};
static const unichar DT_0400[] =
{
    1 /*character count*/,
    0x0044 /*LATIN CAPITAL LETTER D*/,
};
static const unichar DT_0401[] =
{
    1 /*character count*/,
    0x0045 /*LATIN CAPITAL LETTER E*/,
};
static const unichar DT_0402[] =
{
    1 /*character count*/,
    0x0046 /*LATIN CAPITAL LETTER F*/,
};
static const unichar DT_0403[] =
{
    1 /*character count*/,
    0x0047 /*LATIN CAPITAL LETTER G*/,
};
static const unichar DT_0404[] =
{
    1 /*character count*/,
    0x0048 /*LATIN CAPITAL LETTER H*/,
};
static const unichar DT_0405[] =
{
    1 /*character count*/,
    0x0049 /*LATIN CAPITAL LETTER I*/,
};
static const unichar DT_0406[] =
{
    1 /*character count*/,
    0x004a /*LATIN CAPITAL LETTER J*/,
};
static const unichar DT_0407[] =
{
    1 /*character count*/,
    0x004b /*LATIN CAPITAL LETTER K*/,
};
static const unichar DT_0408[] =
{
    1 /*character count*/,
    0x004c /*LATIN CAPITAL LETTER L*/,
};
static const unichar DT_0409[] =
{
    1 /*character count*/,
    0x004d /*LATIN CAPITAL LETTER M*/,
};
static const unichar DT_0410[] =
{
    1 /*character count*/,
    0x004e /*LATIN CAPITAL LETTER N*/,
};
static const unichar DT_0411[] =
{
    1 /*character count*/,
    0x004f /*LATIN CAPITAL LETTER O*/,
};
static const unichar DT_0412[] =
{
    1 /*character count*/,
    0x0050 /*LATIN CAPITAL LETTER P*/,
};
static const unichar DT_0413[] =
{
    1 /*character count*/,
    0x0051 /*LATIN CAPITAL LETTER Q*/,
};
static const unichar DT_0414[] =
{
    1 /*character count*/,
    0x0052 /*LATIN CAPITAL LETTER R*/,
};
static const unichar DT_0415[] =
{
    1 /*character count*/,
    0x0053 /*LATIN CAPITAL LETTER S*/,
};
static const unichar DT_0416[] =
{
    1 /*character count*/,
    0x0054 /*LATIN CAPITAL LETTER T*/,
};
static const unichar DT_0417[] =
{
    1 /*character count*/,
    0x0055 /*LATIN CAPITAL LETTER U*/,
};
static const unichar DT_0418[] =
{
    1 /*character count*/,
    0x0056 /*LATIN CAPITAL LETTER V*/,
};
static const unichar DT_0419[] =
{
    1 /*character count*/,
    0x0057 /*LATIN CAPITAL LETTER W*/,
};
static const unichar DT_0420[] =
{
    1 /*character count*/,
    0x0058 /*LATIN CAPITAL LETTER X*/,
};
static const unichar DT_0421[] =
{
    1 /*character count*/,
    0x0059 /*LATIN CAPITAL LETTER Y*/,
};
static const unichar DT_0422[] =
{
    1 /*character count*/,
    0x005a /*LATIN CAPITAL LETTER Z*/,
};
static const unichar DT_0423[] =
{
    1 /*character count*/,
    0x005b /*LEFT SQUARE BRACKET*/,
};
static const unichar DT_0424[] =
{
    1 /*character count*/,
    0x005c /*REVERSE SOLIDUS*/,
};
static const unichar DT_0425[] =
{
    1 /*character count*/,
    0x005d /*RIGHT SQUARE BRACKET*/,
};
static const unichar DT_0426[] =
{
    1 /*character count*/,
    0x005e /*CIRCUMFLEX ACCENT*/,
};
static const unichar DT_0427[] =
{
    1 /*character count*/,
    0x005f /*LOW LINE*/,
};
static const unichar DT_0428[] =
{
    1 /*character count*/,
    0x0060 /*GRAVE ACCENT*/,
};
static const unichar DT_0429[] =
{
    1 /*character count*/,
    0x0062 /*LATIN SMALL LETTER B*/,
};
static const unichar DT_0430[] =
{
    1 /*character count*/,
    0x0063 /*LATIN SMALL LETTER C*/,
};
static const unichar DT_0431[] =
{
    1 /*character count*/,
    0x0064 /*LATIN SMALL LETTER D*/,
};
static const unichar DT_0432[] =
{
    1 /*character count*/,
    0x0065 /*LATIN SMALL LETTER E*/,
};
static const unichar DT_0433[] =
{
    1 /*character count*/,
    0x0066 /*LATIN SMALL LETTER F*/,
};
static const unichar DT_0434[] =
{
    1 /*character count*/,
    0x0067 /*LATIN SMALL LETTER G*/,
};
static const unichar DT_0435[] =
{
    1 /*character count*/,
    0x0068 /*LATIN SMALL LETTER H*/,
};
static const unichar DT_0436[] =
{
    1 /*character count*/,
    0x0069 /*LATIN SMALL LETTER I*/,
};
static const unichar DT_0437[] =
{
    1 /*character count*/,
    0x006a /*LATIN SMALL LETTER J*/,
};
static const unichar DT_0438[] =
{
    1 /*character count*/,
    0x006b /*LATIN SMALL LETTER K*/,
};
static const unichar DT_0439[] =
{
    1 /*character count*/,
    0x006c /*LATIN SMALL LETTER L*/,
};
static const unichar DT_0440[] =
{
    1 /*character count*/,
    0x006d /*LATIN SMALL LETTER M*/,
};
static const unichar DT_0441[] =
{
    1 /*character count*/,
    0x006e /*LATIN SMALL LETTER N*/,
};
static const unichar DT_0442[] =
{
    1 /*character count*/,
    0x0070 /*LATIN SMALL LETTER P*/,
};
static const unichar DT_0443[] =
{
    1 /*character count*/,
    0x0071 /*LATIN SMALL LETTER Q*/,
};
static const unichar DT_0444[] =
{
    1 /*character count*/,
    0x0072 /*LATIN SMALL LETTER R*/,
};
static const unichar DT_0445[] =
{
    1 /*character count*/,
    0x0074 /*LATIN SMALL LETTER T*/,
};
static const unichar DT_0446[] =
{
    1 /*character count*/,
    0x0075 /*LATIN SMALL LETTER U*/,
};
static const unichar DT_0447[] =
{
    1 /*character count*/,
    0x0076 /*LATIN SMALL LETTER V*/,
};
static const unichar DT_0448[] =
{
    1 /*character count*/,
    0x0077 /*LATIN SMALL LETTER W*/,
};
static const unichar DT_0449[] =
{
    1 /*character count*/,
    0x0078 /*LATIN SMALL LETTER X*/,
};
static const unichar DT_0450[] =
{
    1 /*character count*/,
    0x0079 /*LATIN SMALL LETTER Y*/,
};
static const unichar DT_0451[] =
{
    1 /*character count*/,
    0x007a /*LATIN SMALL LETTER Z*/,
};
static const unichar DT_0452[] =
{
    1 /*character count*/,
    0x007b /*LEFT CURLY BRACKET*/,
};
static const unichar DT_0453[] =
{
    1 /*character count*/,
    0x007c /*VERTICAL LINE*/,
};
static const unichar DT_0454[] =
{
    1 /*character count*/,
    0x007d /*RIGHT CURLY BRACKET*/,
};
static const unichar DT_0455[] =
{
    1 /*character count*/,
    0x007e /*TILDE*/,
};
static const unichar DT_0456[] =
{
    1 /*character count*/,
    0x30fb /*KATAKANA MIDDLE DOT*/,
};
static const unichar DT_0457[] =
{
    1 /*character count*/,
    0x30fc /*KATAKANA-HIRAGANA PROLONGED SOUND MARK*/,
};
static const unichar DT_0458[] =
{
    1 /*character count*/,
    0x3099 /*COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK*/,
};
static const unichar DT_0459[] =
{
    1 /*character count*/,
    0x309a /*COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK*/,
};

unichar const* const UnicharDecompositionData_Latin[] =
{
    /* NO-BREAK SPACE */ DT_0000,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* DIAERESIS */ DT_0001,
    /* dummy */ NULL,
    /* FEMININE ORDINAL INDICATOR */ DT_0002,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* MACRON */ DT_0003,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* SUPERSCRIPT TWO */ DT_0004,
    /* SUPERSCRIPT THREE */ DT_0005,
    /* ACUTE ACCENT */ DT_0006,
    /* MICRO SIGN */ DT_0007,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* CEDILLA */ DT_0008,
    /* SUPERSCRIPT ONE */ DT_0009,
    /* MASCULINE ORDINAL INDICATOR */ DT_0010,
    /* dummy */ NULL,
    /* VULGAR FRACTION ONE QUARTER */ DT_0011,
    /* VULGAR FRACTION ONE HALF */ DT_0012,
    /* VULGAR FRACTION THREE QUARTERS */ DT_0013,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER A WITH GRAVE */ DT_0014,
    /* LATIN CAPITAL LETTER A WITH ACUTE */ DT_0015,
    /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */ DT_0016,
    /* LATIN CAPITAL LETTER A WITH TILDE */ DT_0017,
    /* LATIN CAPITAL LETTER A WITH DIAERESIS */ DT_0018,
    /* LATIN CAPITAL LETTER A WITH RING ABOVE */ DT_0019,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER C WITH CEDILLA */ DT_0020,
    /* LATIN CAPITAL LETTER E WITH GRAVE */ DT_0021,
    /* LATIN CAPITAL LETTER E WITH ACUTE */ DT_0022,
    /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */ DT_0023,
    /* LATIN CAPITAL LETTER E WITH DIAERESIS */ DT_0024,
    /* LATIN CAPITAL LETTER I WITH GRAVE */ DT_0025,
    /* LATIN CAPITAL LETTER I WITH ACUTE */ DT_0026,
    /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */ DT_0027,
    /* LATIN CAPITAL LETTER I WITH DIAERESIS */ DT_0028,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER N WITH TILDE */ DT_0029,
    /* LATIN CAPITAL LETTER O WITH GRAVE */ DT_0030,
    /* LATIN CAPITAL LETTER O WITH ACUTE */ DT_0031,
    /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */ DT_0032,
    /* LATIN CAPITAL LETTER O WITH TILDE */ DT_0033,
    /* LATIN CAPITAL LETTER O WITH DIAERESIS */ DT_0034,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER U WITH GRAVE */ DT_0035,
    /* LATIN CAPITAL LETTER U WITH ACUTE */ DT_0036,
    /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */ DT_0037,
    /* LATIN CAPITAL LETTER U WITH DIAERESIS */ DT_0038,
    /* LATIN CAPITAL LETTER Y WITH ACUTE */ DT_0039,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN SMALL LETTER A WITH GRAVE */ DT_0040,
    /* LATIN SMALL LETTER A WITH ACUTE */ DT_0041,
    /* LATIN SMALL LETTER A WITH CIRCUMFLEX */ DT_0042,
    /* LATIN SMALL LETTER A WITH TILDE */ DT_0043,
    /* LATIN SMALL LETTER A WITH DIAERESIS */ DT_0044,
    /* LATIN SMALL LETTER A WITH RING ABOVE */ DT_0045,
    /* dummy */ NULL,
    /* LATIN SMALL LETTER C WITH CEDILLA */ DT_0046,
    /* LATIN SMALL LETTER E WITH GRAVE */ DT_0047,
    /* LATIN SMALL LETTER E WITH ACUTE */ DT_0048,
    /* LATIN SMALL LETTER E WITH CIRCUMFLEX */ DT_0049,
    /* LATIN SMALL LETTER E WITH DIAERESIS */ DT_0050,
    /* LATIN SMALL LETTER I WITH GRAVE */ DT_0051,
    /* LATIN SMALL LETTER I WITH ACUTE */ DT_0052,
    /* LATIN SMALL LETTER I WITH CIRCUMFLEX */ DT_0053,
    /* LATIN SMALL LETTER I WITH DIAERESIS */ DT_0054,
    /* dummy */ NULL,
    /* LATIN SMALL LETTER N WITH TILDE */ DT_0055,
    /* LATIN SMALL LETTER O WITH GRAVE */ DT_0056,
    /* LATIN SMALL LETTER O WITH ACUTE */ DT_0057,
    /* LATIN SMALL LETTER O WITH CIRCUMFLEX */ DT_0058,
    /* LATIN SMALL LETTER O WITH TILDE */ DT_0059,
    /* LATIN SMALL LETTER O WITH DIAERESIS */ DT_0060,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN SMALL LETTER U WITH GRAVE */ DT_0061,
    /* LATIN SMALL LETTER U WITH ACUTE */ DT_0062,
    /* LATIN SMALL LETTER U WITH CIRCUMFLEX */ DT_0063,
    /* LATIN SMALL LETTER U WITH DIAERESIS */ DT_0064,
    /* LATIN SMALL LETTER Y WITH ACUTE */ DT_0065,
    /* dummy */ NULL,
    /* LATIN SMALL LETTER Y WITH DIAERESIS */ DT_0066,
    /* LATIN CAPITAL LETTER A WITH MACRON */ DT_0067,
    /* LATIN SMALL LETTER A WITH MACRON */ DT_0068,
    /* LATIN CAPITAL LETTER A WITH BREVE */ DT_0069,
    /* LATIN SMALL LETTER A WITH BREVE */ DT_0070,
    /* LATIN CAPITAL LETTER A WITH OGONEK */ DT_0071,
    /* LATIN SMALL LETTER A WITH OGONEK */ DT_0072,
    /* LATIN CAPITAL LETTER C WITH ACUTE */ DT_0073,
    /* LATIN SMALL LETTER C WITH ACUTE */ DT_0074,
    /* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */ DT_0075,
    /* LATIN SMALL LETTER C WITH CIRCUMFLEX */ DT_0076,
    /* LATIN CAPITAL LETTER C WITH DOT ABOVE */ DT_0077,
    /* LATIN SMALL LETTER C WITH DOT ABOVE */ DT_0078,
    /* LATIN CAPITAL LETTER C WITH CARON */ DT_0079,
    /* LATIN SMALL LETTER C WITH CARON */ DT_0080,
    /* LATIN CAPITAL LETTER D WITH CARON */ DT_0081,
    /* LATIN SMALL LETTER D WITH CARON */ DT_0082,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER E WITH MACRON */ DT_0083,
    /* LATIN SMALL LETTER E WITH MACRON */ DT_0084,
    /* LATIN CAPITAL LETTER E WITH BREVE */ DT_0085,
    /* LATIN SMALL LETTER E WITH BREVE */ DT_0086,
    /* LATIN CAPITAL LETTER E WITH DOT ABOVE */ DT_0087,
    /* LATIN SMALL LETTER E WITH DOT ABOVE */ DT_0088,
    /* LATIN CAPITAL LETTER E WITH OGONEK */ DT_0089,
    /* LATIN SMALL LETTER E WITH OGONEK */ DT_0090,
    /* LATIN CAPITAL LETTER E WITH CARON */ DT_0091,
    /* LATIN SMALL LETTER E WITH CARON */ DT_0092,
    /* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */ DT_0093,
    /* LATIN SMALL LETTER G WITH CIRCUMFLEX */ DT_0094,
    /* LATIN CAPITAL LETTER G WITH BREVE */ DT_0095,
    /* LATIN SMALL LETTER G WITH BREVE */ DT_0096,
    /* LATIN CAPITAL LETTER G WITH DOT ABOVE */ DT_0097,
    /* LATIN SMALL LETTER G WITH DOT ABOVE */ DT_0098,
    /* LATIN CAPITAL LETTER G WITH CEDILLA */ DT_0099,
    /* LATIN SMALL LETTER G WITH CEDILLA */ DT_0100,
    /* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */ DT_0101,
    /* LATIN SMALL LETTER H WITH CIRCUMFLEX */ DT_0102,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER I WITH TILDE */ DT_0103,
    /* LATIN SMALL LETTER I WITH TILDE */ DT_0104,
    /* LATIN CAPITAL LETTER I WITH MACRON */ DT_0105,
    /* LATIN SMALL LETTER I WITH MACRON */ DT_0106,
    /* LATIN CAPITAL LETTER I WITH BREVE */ DT_0107,
    /* LATIN SMALL LETTER I WITH BREVE */ DT_0108,
    /* LATIN CAPITAL LETTER I WITH OGONEK */ DT_0109,
    /* LATIN SMALL LETTER I WITH OGONEK */ DT_0110,
    /* LATIN CAPITAL LETTER I WITH DOT ABOVE */ DT_0111,
    /* dummy */ NULL,
    /* LATIN CAPITAL LIGATURE IJ */ DT_0112,
    /* LATIN SMALL LIGATURE IJ */ DT_0113,
    /* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */ DT_0114,
    /* LATIN SMALL LETTER J WITH CIRCUMFLEX */ DT_0115,
    /* LATIN CAPITAL LETTER K WITH CEDILLA */ DT_0116,
    /* LATIN SMALL LETTER K WITH CEDILLA */ DT_0117,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER L WITH ACUTE */ DT_0118,
    /* LATIN SMALL LETTER L WITH ACUTE */ DT_0119,
    /* LATIN CAPITAL LETTER L WITH CEDILLA */ DT_0120,
    /* LATIN SMALL LETTER L WITH CEDILLA */ DT_0121,
    /* LATIN CAPITAL LETTER L WITH CARON */ DT_0122,
    /* LATIN SMALL LETTER L WITH CARON */ DT_0123,
    /* LATIN CAPITAL LETTER L WITH MIDDLE DOT */ DT_0124,
    /* LATIN SMALL LETTER L WITH MIDDLE DOT */ DT_0125,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER N WITH ACUTE */ DT_0126,
    /* LATIN SMALL LETTER N WITH ACUTE */ DT_0127,
    /* LATIN CAPITAL LETTER N WITH CEDILLA */ DT_0128,
    /* LATIN SMALL LETTER N WITH CEDILLA */ DT_0129,
    /* LATIN CAPITAL LETTER N WITH CARON */ DT_0130,
    /* LATIN SMALL LETTER N WITH CARON */ DT_0131,
    /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */ DT_0132,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER O WITH MACRON */ DT_0133,
    /* LATIN SMALL LETTER O WITH MACRON */ DT_0134,
    /* LATIN CAPITAL LETTER O WITH BREVE */ DT_0135,
    /* LATIN SMALL LETTER O WITH BREVE */ DT_0136,
    /* LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */ DT_0137,
    /* LATIN SMALL LETTER O WITH DOUBLE ACUTE */ DT_0138,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER R WITH ACUTE */ DT_0139,
    /* LATIN SMALL LETTER R WITH ACUTE */ DT_0140,
    /* LATIN CAPITAL LETTER R WITH CEDILLA */ DT_0141,
    /* LATIN SMALL LETTER R WITH CEDILLA */ DT_0142,
    /* LATIN CAPITAL LETTER R WITH CARON */ DT_0143,
    /* LATIN SMALL LETTER R WITH CARON */ DT_0144,
    /* LATIN CAPITAL LETTER S WITH ACUTE */ DT_0145,
    /* LATIN SMALL LETTER S WITH ACUTE */ DT_0146,
    /* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */ DT_0147,
    /* LATIN SMALL LETTER S WITH CIRCUMFLEX */ DT_0148,
    /* LATIN CAPITAL LETTER S WITH CEDILLA */ DT_0149,
    /* LATIN SMALL LETTER S WITH CEDILLA */ DT_0150,
    /* LATIN CAPITAL LETTER S WITH CARON */ DT_0151,
    /* LATIN SMALL LETTER S WITH CARON */ DT_0152,
    /* LATIN CAPITAL LETTER T WITH CEDILLA */ DT_0153,
    /* LATIN SMALL LETTER T WITH CEDILLA */ DT_0154,
    /* LATIN CAPITAL LETTER T WITH CARON */ DT_0155,
    /* LATIN SMALL LETTER T WITH CARON */ DT_0156,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER U WITH TILDE */ DT_0157,
    /* LATIN SMALL LETTER U WITH TILDE */ DT_0158,
    /* LATIN CAPITAL LETTER U WITH MACRON */ DT_0159,
    /* LATIN SMALL LETTER U WITH MACRON */ DT_0160,
    /* LATIN CAPITAL LETTER U WITH BREVE */ DT_0161,
    /* LATIN SMALL LETTER U WITH BREVE */ DT_0162,
    /* LATIN CAPITAL LETTER U WITH RING ABOVE */ DT_0163,
    /* LATIN SMALL LETTER U WITH RING ABOVE */ DT_0164,
    /* LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */ DT_0165,
    /* LATIN SMALL LETTER U WITH DOUBLE ACUTE */ DT_0166,
    /* LATIN CAPITAL LETTER U WITH OGONEK */ DT_0167,
    /* LATIN SMALL LETTER U WITH OGONEK */ DT_0168,
    /* LATIN CAPITAL LETTER W WITH CIRCUMFLEX */ DT_0169,
    /* LATIN SMALL LETTER W WITH CIRCUMFLEX */ DT_0170,
    /* LATIN CAPITAL LETTER Y WITH CIRCUMFLEX */ DT_0171,
    /* LATIN SMALL LETTER Y WITH CIRCUMFLEX */ DT_0172,
    /* LATIN CAPITAL LETTER Y WITH DIAERESIS */ DT_0173,
    /* LATIN CAPITAL LETTER Z WITH ACUTE */ DT_0174,
    /* LATIN SMALL LETTER Z WITH ACUTE */ DT_0175,
    /* LATIN CAPITAL LETTER Z WITH DOT ABOVE */ DT_0176,
    /* LATIN SMALL LETTER Z WITH DOT ABOVE */ DT_0177,
    /* LATIN CAPITAL LETTER Z WITH CARON */ DT_0178,
    /* LATIN SMALL LETTER Z WITH CARON */ DT_0179,
    /* LATIN SMALL LETTER LONG S */ DT_0180,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER O WITH HORN */ DT_0181,
    /* LATIN SMALL LETTER O WITH HORN */ DT_0182,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER U WITH HORN */ DT_0183,
    /* LATIN SMALL LETTER U WITH HORN */ DT_0184,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER DZ WITH CARON */ DT_0185,
    /* LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON */ DT_0186,
    /* LATIN SMALL LETTER DZ WITH CARON */ DT_0187,
    /* LATIN CAPITAL LETTER LJ */ DT_0188,
    /* LATIN CAPITAL LETTER L WITH SMALL LETTER J */ DT_0189,
    /* LATIN SMALL LETTER LJ */ DT_0190,
    /* LATIN CAPITAL LETTER NJ */ DT_0191,
    /* LATIN CAPITAL LETTER N WITH SMALL LETTER J */ DT_0192,
    /* LATIN SMALL LETTER NJ */ DT_0193,
    /* LATIN CAPITAL LETTER A WITH CARON */ DT_0194,
    /* LATIN SMALL LETTER A WITH CARON */ DT_0195,
    /* LATIN CAPITAL LETTER I WITH CARON */ DT_0196,
    /* LATIN SMALL LETTER I WITH CARON */ DT_0197,
    /* LATIN CAPITAL LETTER O WITH CARON */ DT_0198,
    /* LATIN SMALL LETTER O WITH CARON */ DT_0199,
    /* LATIN CAPITAL LETTER U WITH CARON */ DT_0200,
    /* LATIN SMALL LETTER U WITH CARON */ DT_0201,
    /* LATIN CAPITAL LETTER U WITH DIAERESIS AND MACRON */ DT_0202,
    /* LATIN SMALL LETTER U WITH DIAERESIS AND MACRON */ DT_0203,
    /* LATIN CAPITAL LETTER U WITH DIAERESIS AND ACUTE */ DT_0204,
    /* LATIN SMALL LETTER U WITH DIAERESIS AND ACUTE */ DT_0205,
    /* LATIN CAPITAL LETTER U WITH DIAERESIS AND CARON */ DT_0206,
    /* LATIN SMALL LETTER U WITH DIAERESIS AND CARON */ DT_0207,
    /* LATIN CAPITAL LETTER U WITH DIAERESIS AND GRAVE */ DT_0208,
    /* LATIN SMALL LETTER U WITH DIAERESIS AND GRAVE */ DT_0209,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER A WITH DIAERESIS AND MACRON */ DT_0210,
    /* LATIN SMALL LETTER A WITH DIAERESIS AND MACRON */ DT_0211,
    /* LATIN CAPITAL LETTER A WITH DOT ABOVE AND MACRON */ DT_0212,
    /* LATIN SMALL LETTER A WITH DOT ABOVE AND MACRON */ DT_0213,
    /* LATIN CAPITAL LETTER AE WITH MACRON */ DT_0214,
    /* LATIN SMALL LETTER AE WITH MACRON */ DT_0215,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER G WITH CARON */ DT_0216,
    /* LATIN SMALL LETTER G WITH CARON */ DT_0217,
    /* LATIN CAPITAL LETTER K WITH CARON */ DT_0218,
    /* LATIN SMALL LETTER K WITH CARON */ DT_0219,
    /* LATIN CAPITAL LETTER O WITH OGONEK */ DT_0220,
    /* LATIN SMALL LETTER O WITH OGONEK */ DT_0221,
    /* LATIN CAPITAL LETTER O WITH OGONEK AND MACRON */ DT_0222,
    /* LATIN SMALL LETTER O WITH OGONEK AND MACRON */ DT_0223,
    /* LATIN CAPITAL LETTER EZH WITH CARON */ DT_0224,
    /* LATIN SMALL LETTER EZH WITH CARON */ DT_0225,
    /* LATIN SMALL LETTER J WITH CARON */ DT_0226,
    /* LATIN CAPITAL LETTER DZ */ DT_0227,
    /* LATIN CAPITAL LETTER D WITH SMALL LETTER Z */ DT_0228,
    /* LATIN SMALL LETTER DZ */ DT_0229,
    /* LATIN CAPITAL LETTER G WITH ACUTE */ DT_0230,
    /* LATIN SMALL LETTER G WITH ACUTE */ DT_0231,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER N WITH GRAVE */ DT_0232,
    /* LATIN SMALL LETTER N WITH GRAVE */ DT_0233,
    /* LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE */ DT_0234,
    /* LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE */ DT_0235,
    /* LATIN CAPITAL LETTER AE WITH ACUTE */ DT_0236,
    /* LATIN SMALL LETTER AE WITH ACUTE */ DT_0237,
    /* LATIN CAPITAL LETTER O WITH STROKE AND ACUTE */ DT_0238,
    /* LATIN SMALL LETTER O WITH STROKE AND ACUTE */ DT_0239,
    /* LATIN CAPITAL LETTER A WITH DOUBLE GRAVE */ DT_0240,
    /* LATIN SMALL LETTER A WITH DOUBLE GRAVE */ DT_0241,
    /* LATIN CAPITAL LETTER A WITH INVERTED BREVE */ DT_0242,
    /* LATIN SMALL LETTER A WITH INVERTED BREVE */ DT_0243,
    /* LATIN CAPITAL LETTER E WITH DOUBLE GRAVE */ DT_0244,
    /* LATIN SMALL LETTER E WITH DOUBLE GRAVE */ DT_0245,
    /* LATIN CAPITAL LETTER E WITH INVERTED BREVE */ DT_0246,
    /* LATIN SMALL LETTER E WITH INVERTED BREVE */ DT_0247,
    /* LATIN CAPITAL LETTER I WITH DOUBLE GRAVE */ DT_0248,
    /* LATIN SMALL LETTER I WITH DOUBLE GRAVE */ DT_0249,
    /* LATIN CAPITAL LETTER I WITH INVERTED BREVE */ DT_0250,
    /* LATIN SMALL LETTER I WITH INVERTED BREVE */ DT_0251,
    /* LATIN CAPITAL LETTER O WITH DOUBLE GRAVE */ DT_0252,
    /* LATIN SMALL LETTER O WITH DOUBLE GRAVE */ DT_0253,
    /* LATIN CAPITAL LETTER O WITH INVERTED BREVE */ DT_0254,
    /* LATIN SMALL LETTER O WITH INVERTED BREVE */ DT_0255,
    /* LATIN CAPITAL LETTER R WITH DOUBLE GRAVE */ DT_0256,
    /* LATIN SMALL LETTER R WITH DOUBLE GRAVE */ DT_0257,
    /* LATIN CAPITAL LETTER R WITH INVERTED BREVE */ DT_0258,
    /* LATIN SMALL LETTER R WITH INVERTED BREVE */ DT_0259,
    /* LATIN CAPITAL LETTER U WITH DOUBLE GRAVE */ DT_0260,
    /* LATIN SMALL LETTER U WITH DOUBLE GRAVE */ DT_0261,
    /* LATIN CAPITAL LETTER U WITH INVERTED BREVE */ DT_0262,
    /* LATIN SMALL LETTER U WITH INVERTED BREVE */ DT_0263,
    /* LATIN CAPITAL LETTER S WITH COMMA BELOW */ DT_0264,
    /* LATIN SMALL LETTER S WITH COMMA BELOW */ DT_0265,
    /* LATIN CAPITAL LETTER T WITH COMMA BELOW */ DT_0266,
    /* LATIN SMALL LETTER T WITH COMMA BELOW */ DT_0267,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER H WITH CARON */ DT_0268,
    /* LATIN SMALL LETTER H WITH CARON */ DT_0269,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* LATIN CAPITAL LETTER A WITH DOT ABOVE */ DT_0270,
    /* LATIN SMALL LETTER A WITH DOT ABOVE */ DT_0271,
    /* LATIN CAPITAL LETTER E WITH CEDILLA */ DT_0272,
    /* LATIN SMALL LETTER E WITH CEDILLA */ DT_0273,
    /* LATIN CAPITAL LETTER O WITH DIAERESIS AND MACRON */ DT_0274,
    /* LATIN SMALL LETTER O WITH DIAERESIS AND MACRON */ DT_0275,
    /* LATIN CAPITAL LETTER O WITH TILDE AND MACRON */ DT_0276,
    /* LATIN SMALL LETTER O WITH TILDE AND MACRON */ DT_0277,
    /* LATIN CAPITAL LETTER O WITH DOT ABOVE */ DT_0278,
    /* LATIN SMALL LETTER O WITH DOT ABOVE */ DT_0279,
    /* LATIN CAPITAL LETTER O WITH DOT ABOVE AND MACRON */ DT_0280,
    /* LATIN SMALL LETTER O WITH DOT ABOVE AND MACRON */ DT_0281,
    /* LATIN CAPITAL LETTER Y WITH MACRON */ DT_0282,
    /* LATIN SMALL LETTER Y WITH MACRON */ DT_0283,
};
unichar const* const UnicharDecompositionData_Kana[] =
{
    /* HIRAGANA LETTER SMALL A */ DT_0284,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL I */ DT_0285,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL U */ DT_0286,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL E */ DT_0287,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL O */ DT_0288,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* HIRAGANA LETTER GA */ DT_0289,
    /* dummy */ NULL,
    /* HIRAGANA LETTER GI */ DT_0290,
    /* dummy */ NULL,
    /* HIRAGANA LETTER GU */ DT_0291,
    /* dummy */ NULL,
    /* HIRAGANA LETTER GE */ DT_0292,
    /* dummy */ NULL,
    /* HIRAGANA LETTER GO */ DT_0293,
    /* dummy */ NULL,
    /* HIRAGANA LETTER ZA */ DT_0294,
    /* dummy */ NULL,
    /* HIRAGANA LETTER ZI */ DT_0295,
    /* dummy */ NULL,
    /* HIRAGANA LETTER ZU */ DT_0296,
    /* dummy */ NULL,
    /* HIRAGANA LETTER ZE */ DT_0297,
    /* dummy */ NULL,
    /* HIRAGANA LETTER ZO */ DT_0298,
    /* dummy */ NULL,
    /* HIRAGANA LETTER DA */ DT_0299,
    /* dummy */ NULL,
    /* HIRAGANA LETTER DI */ DT_0300,
    /* HIRAGANA LETTER SMALL TU */ DT_0301,
    /* dummy */ NULL,
    /* HIRAGANA LETTER DU */ DT_0302,
    /* dummy */ NULL,
    /* HIRAGANA LETTER DE */ DT_0303,
    /* dummy */ NULL,
    /* HIRAGANA LETTER DO */ DT_0304,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* HIRAGANA LETTER BA */ DT_0305,
    /* HIRAGANA LETTER PA */ DT_0306,
    /* dummy */ NULL,
    /* HIRAGANA LETTER BI */ DT_0307,
    /* HIRAGANA LETTER PI */ DT_0308,
    /* dummy */ NULL,
    /* HIRAGANA LETTER BU */ DT_0309,
    /* HIRAGANA LETTER PU */ DT_0310,
    /* dummy */ NULL,
    /* HIRAGANA LETTER BE */ DT_0311,
    /* HIRAGANA LETTER PE */ DT_0312,
    /* dummy */ NULL,
    /* HIRAGANA LETTER BO */ DT_0313,
    /* HIRAGANA LETTER PO */ DT_0314,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL YA */ DT_0315,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL YU */ DT_0316,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL YO */ DT_0317,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* HIRAGANA LETTER SMALL WA */ DT_0318,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* HIRAGANA LETTER VU */ DT_0319,
    /* HIRAGANA LETTER SMALL KA */ DT_0320,
    /* HIRAGANA LETTER SMALL KE */ DT_0321,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* KATAKANA-HIRAGANA VOICED SOUND MARK */ DT_0322,
    /* KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK */ DT_0323,
    /* dummy */ NULL,
    /* HIRAGANA VOICED ITERATION MARK */ DT_0324,
    /* HIRAGANA DIGRAPH YORI */ DT_0325,
    /* dummy */ NULL,
    /* KATAKANA LETTER SMALL A */ DT_0284,
    /* KATAKANA LETTER A */ DT_0284,
    /* KATAKANA LETTER SMALL I */ DT_0285,
    /* KATAKANA LETTER I */ DT_0285,
    /* KATAKANA LETTER SMALL U */ DT_0286,
    /* KATAKANA LETTER U */ DT_0286,
    /* KATAKANA LETTER SMALL E */ DT_0287,
    /* KATAKANA LETTER E */ DT_0287,
    /* KATAKANA LETTER SMALL O */ DT_0288,
    /* KATAKANA LETTER O */ DT_0288,
    /* KATAKANA LETTER KA */ DT_0320,
    /* KATAKANA LETTER GA */ DT_0289,
    /* KATAKANA LETTER KI */ DT_0326,
    /* KATAKANA LETTER GI */ DT_0290,
    /* KATAKANA LETTER KU */ DT_0327,
    /* KATAKANA LETTER GU */ DT_0291,
    /* KATAKANA LETTER KE */ DT_0321,
    /* KATAKANA LETTER GE */ DT_0292,
    /* KATAKANA LETTER KO */ DT_0328,
    /* KATAKANA LETTER GO */ DT_0293,
    /* KATAKANA LETTER SA */ DT_0329,
    /* KATAKANA LETTER ZA */ DT_0294,
    /* KATAKANA LETTER SI */ DT_0330,
    /* KATAKANA LETTER ZI */ DT_0295,
    /* KATAKANA LETTER SU */ DT_0331,
    /* KATAKANA LETTER ZU */ DT_0296,
    /* KATAKANA LETTER SE */ DT_0332,
    /* KATAKANA LETTER ZE */ DT_0297,
    /* KATAKANA LETTER SO */ DT_0333,
    /* KATAKANA LETTER ZO */ DT_0298,
    /* KATAKANA LETTER TA */ DT_0334,
    /* KATAKANA LETTER DA */ DT_0299,
    /* KATAKANA LETTER TI */ DT_0335,
    /* KATAKANA LETTER DI */ DT_0300,
    /* KATAKANA LETTER SMALL TU */ DT_0301,
    /* KATAKANA LETTER TU */ DT_0301,
    /* KATAKANA LETTER DU */ DT_0302,
    /* KATAKANA LETTER TE */ DT_0336,
    /* KATAKANA LETTER DE */ DT_0303,
    /* KATAKANA LETTER TO */ DT_0337,
    /* KATAKANA LETTER DO */ DT_0304,
    /* KATAKANA LETTER NA */ DT_0338,
    /* KATAKANA LETTER NI */ DT_0339,
    /* KATAKANA LETTER NU */ DT_0340,
    /* KATAKANA LETTER NE */ DT_0341,
    /* KATAKANA LETTER NO */ DT_0342,
    /* KATAKANA LETTER HA */ DT_0343,
    /* KATAKANA LETTER BA */ DT_0305,
    /* KATAKANA LETTER PA */ DT_0306,
    /* KATAKANA LETTER HI */ DT_0344,
    /* KATAKANA LETTER BI */ DT_0307,
    /* KATAKANA LETTER PI */ DT_0308,
    /* KATAKANA LETTER HU */ DT_0345,
    /* KATAKANA LETTER BU */ DT_0309,
    /* KATAKANA LETTER PU */ DT_0310,
    /* KATAKANA LETTER HE */ DT_0346,
    /* KATAKANA LETTER BE */ DT_0311,
    /* KATAKANA LETTER PE */ DT_0312,
    /* KATAKANA LETTER HO */ DT_0347,
    /* KATAKANA LETTER BO */ DT_0313,
    /* KATAKANA LETTER PO */ DT_0314,
    /* KATAKANA LETTER MA */ DT_0348,
    /* KATAKANA LETTER MI */ DT_0349,
    /* KATAKANA LETTER MU */ DT_0350,
    /* KATAKANA LETTER ME */ DT_0351,
    /* KATAKANA LETTER MO */ DT_0352,
    /* KATAKANA LETTER SMALL YA */ DT_0315,
    /* KATAKANA LETTER YA */ DT_0315,
    /* KATAKANA LETTER SMALL YU */ DT_0316,
    /* KATAKANA LETTER YU */ DT_0316,
    /* KATAKANA LETTER SMALL YO */ DT_0317,
    /* KATAKANA LETTER YO */ DT_0317,
    /* KATAKANA LETTER RA */ DT_0353,
    /* KATAKANA LETTER RI */ DT_0354,
    /* KATAKANA LETTER RU */ DT_0355,
    /* KATAKANA LETTER RE */ DT_0356,
    /* KATAKANA LETTER RO */ DT_0357,
    /* KATAKANA LETTER SMALL WA */ DT_0318,
    /* KATAKANA LETTER WA */ DT_0318,
    /* KATAKANA LETTER WI */ DT_0358,
    /* KATAKANA LETTER WE */ DT_0359,
    /* KATAKANA LETTER WO */ DT_0360,
    /* KATAKANA LETTER N */ DT_0361,
    /* KATAKANA LETTER VU */ DT_0319,
    /* KATAKANA LETTER SMALL KA */ DT_0320,
    /* KATAKANA LETTER SMALL KE */ DT_0321,
    /* KATAKANA LETTER VA */ DT_0362,
    /* KATAKANA LETTER VI */ DT_0363,
    /* KATAKANA LETTER VE */ DT_0364,
    /* KATAKANA LETTER VO */ DT_0365,
    /* dummy */ NULL,
    /* dummy */ NULL,
    /* KATAKANA ITERATION MARK */ DT_0366,
    /* KATAKANA VOICED ITERATION MARK */ DT_0324,
    /* KATAKANA DIGRAPH KOTO */ DT_0367,
};
unichar const* const UnicharDecompositionData_KatakanaPhoneticExtensions[] =
{
    /* KATAKANA LETTER SMALL KU */ DT_0327,
    /* KATAKANA LETTER SMALL SI */ DT_0330,
    /* KATAKANA LETTER SMALL SU */ DT_0331,
    /* KATAKANA LETTER SMALL TO */ DT_0337,
    /* KATAKANA LETTER SMALL NU */ DT_0340,
    /* KATAKANA LETTER SMALL HA */ DT_0343,
    /* KATAKANA LETTER SMALL HI */ DT_0344,
    /* KATAKANA LETTER SMALL HU */ DT_0345,
    /* KATAKANA LETTER SMALL HE */ DT_0346,
    /* KATAKANA LETTER SMALL HO */ DT_0347,
    /* KATAKANA LETTER SMALL MU */ DT_0350,
    /* KATAKANA LETTER SMALL RA */ DT_0353,
    /* KATAKANA LETTER SMALL RI */ DT_0354,
    /* KATAKANA LETTER SMALL RU */ DT_0355,
    /* KATAKANA LETTER SMALL RE */ DT_0356,
    /* KATAKANA LETTER SMALL RO */ DT_0357,
};
unichar const* const UnicharDecompositionData_CircledKatakana[] =
{
    /* CIRCLED KATAKANA A */ DT_0284,
    /* CIRCLED KATAKANA I */ DT_0285,
    /* CIRCLED KATAKANA U */ DT_0286,
    /* CIRCLED KATAKANA E */ DT_0287,
    /* CIRCLED KATAKANA O */ DT_0288,
    /* CIRCLED KATAKANA KA */ DT_0320,
    /* CIRCLED KATAKANA KI */ DT_0326,
    /* CIRCLED KATAKANA KU */ DT_0327,
    /* CIRCLED KATAKANA KE */ DT_0321,
    /* CIRCLED KATAKANA KO */ DT_0328,
    /* CIRCLED KATAKANA SA */ DT_0329,
    /* CIRCLED KATAKANA SI */ DT_0330,
    /* CIRCLED KATAKANA SU */ DT_0331,
    /* CIRCLED KATAKANA SE */ DT_0332,
    /* CIRCLED KATAKANA SO */ DT_0333,
    /* CIRCLED KATAKANA TA */ DT_0334,
    /* CIRCLED KATAKANA TI */ DT_0335,
    /* CIRCLED KATAKANA TU */ DT_0301,
    /* CIRCLED KATAKANA TE */ DT_0336,
    /* CIRCLED KATAKANA TO */ DT_0337,
    /* CIRCLED KATAKANA NA */ DT_0338,
    /* CIRCLED KATAKANA NI */ DT_0339,
    /* CIRCLED KATAKANA NU */ DT_0340,
    /* CIRCLED KATAKANA NE */ DT_0341,
    /* CIRCLED KATAKANA NO */ DT_0342,
    /* CIRCLED KATAKANA HA */ DT_0343,
    /* CIRCLED KATAKANA HI */ DT_0344,
    /* CIRCLED KATAKANA HU */ DT_0345,
    /* CIRCLED KATAKANA HE */ DT_0346,
    /* CIRCLED KATAKANA HO */ DT_0347,
    /* CIRCLED KATAKANA MA */ DT_0348,
    /* CIRCLED KATAKANA MI */ DT_0349,
    /* CIRCLED KATAKANA MU */ DT_0350,
    /* CIRCLED KATAKANA ME */ DT_0351,
    /* CIRCLED KATAKANA MO */ DT_0352,
    /* CIRCLED KATAKANA YA */ DT_0315,
    /* CIRCLED KATAKANA YU */ DT_0316,
    /* CIRCLED KATAKANA YO */ DT_0317,
    /* CIRCLED KATAKANA RA */ DT_0353,
    /* CIRCLED KATAKANA RI */ DT_0354,
    /* CIRCLED KATAKANA RU */ DT_0355,
    /* CIRCLED KATAKANA RE */ DT_0356,
    /* CIRCLED KATAKANA RO */ DT_0357,
    /* CIRCLED KATAKANA WA */ DT_0318,
    /* CIRCLED KATAKANA WI */ DT_0358,
    /* CIRCLED KATAKANA WE */ DT_0359,
    /* CIRCLED KATAKANA WO */ DT_0360,
};
unichar const* const UnicharDecompositionData_FullwidthLatin[] =
{
    /* FULLWIDTH EXCLAMATION MARK */ DT_0368,
    /* FULLWIDTH QUOTATION MARK */ DT_0369,
    /* FULLWIDTH NUMBER SIGN */ DT_0370,
    /* FULLWIDTH DOLLAR SIGN */ DT_0371,
    /* FULLWIDTH PERCENT SIGN */ DT_0372,
    /* FULLWIDTH AMPERSAND */ DT_0373,
    /* FULLWIDTH APOSTROPHE */ DT_0374,
    /* FULLWIDTH LEFT PARENTHESIS */ DT_0375,
    /* FULLWIDTH RIGHT PARENTHESIS */ DT_0376,
    /* FULLWIDTH ASTERISK */ DT_0377,
    /* FULLWIDTH PLUS SIGN */ DT_0378,
    /* FULLWIDTH COMMA */ DT_0379,
    /* FULLWIDTH HYPHEN-MINUS */ DT_0380,
    /* FULLWIDTH FULL STOP */ DT_0381,
    /* FULLWIDTH SOLIDUS */ DT_0382,
    /* FULLWIDTH DIGIT ZERO */ DT_0383,
    /* FULLWIDTH DIGIT ONE */ DT_0009,
    /* FULLWIDTH DIGIT TWO */ DT_0004,
    /* FULLWIDTH DIGIT THREE */ DT_0005,
    /* FULLWIDTH DIGIT FOUR */ DT_0384,
    /* FULLWIDTH DIGIT FIVE */ DT_0385,
    /* FULLWIDTH DIGIT SIX */ DT_0386,
    /* FULLWIDTH DIGIT SEVEN */ DT_0387,
    /* FULLWIDTH DIGIT EIGHT */ DT_0388,
    /* FULLWIDTH DIGIT NINE */ DT_0389,
    /* FULLWIDTH COLON */ DT_0390,
    /* FULLWIDTH SEMICOLON */ DT_0391,
    /* FULLWIDTH LESS-THAN SIGN */ DT_0392,
    /* FULLWIDTH EQUALS SIGN */ DT_0393,
    /* FULLWIDTH GREATER-THAN SIGN */ DT_0394,
    /* FULLWIDTH QUESTION MARK */ DT_0395,
    /* FULLWIDTH COMMERCIAL AT */ DT_0396,
    /* FULLWIDTH LATIN CAPITAL LETTER A */ DT_0397,
    /* FULLWIDTH LATIN CAPITAL LETTER B */ DT_0398,
    /* FULLWIDTH LATIN CAPITAL LETTER C */ DT_0399,
    /* FULLWIDTH LATIN CAPITAL LETTER D */ DT_0400,
    /* FULLWIDTH LATIN CAPITAL LETTER E */ DT_0401,
    /* FULLWIDTH LATIN CAPITAL LETTER F */ DT_0402,
    /* FULLWIDTH LATIN CAPITAL LETTER G */ DT_0403,
    /* FULLWIDTH LATIN CAPITAL LETTER H */ DT_0404,
    /* FULLWIDTH LATIN CAPITAL LETTER I */ DT_0405,
    /* FULLWIDTH LATIN CAPITAL LETTER J */ DT_0406,
    /* FULLWIDTH LATIN CAPITAL LETTER K */ DT_0407,
    /* FULLWIDTH LATIN CAPITAL LETTER L */ DT_0408,
    /* FULLWIDTH LATIN CAPITAL LETTER M */ DT_0409,
    /* FULLWIDTH LATIN CAPITAL LETTER N */ DT_0410,
    /* FULLWIDTH LATIN CAPITAL LETTER O */ DT_0411,
    /* FULLWIDTH LATIN CAPITAL LETTER P */ DT_0412,
    /* FULLWIDTH LATIN CAPITAL LETTER Q */ DT_0413,
    /* FULLWIDTH LATIN CAPITAL LETTER R */ DT_0414,
    /* FULLWIDTH LATIN CAPITAL LETTER S */ DT_0415,
    /* FULLWIDTH LATIN CAPITAL LETTER T */ DT_0416,
    /* FULLWIDTH LATIN CAPITAL LETTER U */ DT_0417,
    /* FULLWIDTH LATIN CAPITAL LETTER V */ DT_0418,
    /* FULLWIDTH LATIN CAPITAL LETTER W */ DT_0419,
    /* FULLWIDTH LATIN CAPITAL LETTER X */ DT_0420,
    /* FULLWIDTH LATIN CAPITAL LETTER Y */ DT_0421,
    /* FULLWIDTH LATIN CAPITAL LETTER Z */ DT_0422,
    /* FULLWIDTH LEFT SQUARE BRACKET */ DT_0423,
    /* FULLWIDTH REVERSE SOLIDUS */ DT_0424,
    /* FULLWIDTH RIGHT SQUARE BRACKET */ DT_0425,
    /* FULLWIDTH CIRCUMFLEX ACCENT */ DT_0426,
    /* FULLWIDTH LOW LINE */ DT_0427,
    /* FULLWIDTH GRAVE ACCENT */ DT_0428,
    /* FULLWIDTH LATIN SMALL LETTER A */ DT_0002,
    /* FULLWIDTH LATIN SMALL LETTER B */ DT_0429,
    /* FULLWIDTH LATIN SMALL LETTER C */ DT_0430,
    /* FULLWIDTH LATIN SMALL LETTER D */ DT_0431,
    /* FULLWIDTH LATIN SMALL LETTER E */ DT_0432,
    /* FULLWIDTH LATIN SMALL LETTER F */ DT_0433,
    /* FULLWIDTH LATIN SMALL LETTER G */ DT_0434,
    /* FULLWIDTH LATIN SMALL LETTER H */ DT_0435,
    /* FULLWIDTH LATIN SMALL LETTER I */ DT_0436,
    /* FULLWIDTH LATIN SMALL LETTER J */ DT_0437,
    /* FULLWIDTH LATIN SMALL LETTER K */ DT_0438,
    /* FULLWIDTH LATIN SMALL LETTER L */ DT_0439,
    /* FULLWIDTH LATIN SMALL LETTER M */ DT_0440,
    /* FULLWIDTH LATIN SMALL LETTER N */ DT_0441,
    /* FULLWIDTH LATIN SMALL LETTER O */ DT_0010,
    /* FULLWIDTH LATIN SMALL LETTER P */ DT_0442,
    /* FULLWIDTH LATIN SMALL LETTER Q */ DT_0443,
    /* FULLWIDTH LATIN SMALL LETTER R */ DT_0444,
    /* FULLWIDTH LATIN SMALL LETTER S */ DT_0180,
    /* FULLWIDTH LATIN SMALL LETTER T */ DT_0445,
    /* FULLWIDTH LATIN SMALL LETTER U */ DT_0446,
    /* FULLWIDTH LATIN SMALL LETTER V */ DT_0447,
    /* FULLWIDTH LATIN SMALL LETTER W */ DT_0448,
    /* FULLWIDTH LATIN SMALL LETTER X */ DT_0449,
    /* FULLWIDTH LATIN SMALL LETTER Y */ DT_0450,
    /* FULLWIDTH LATIN SMALL LETTER Z */ DT_0451,
    /* FULLWIDTH LEFT CURLY BRACKET */ DT_0452,
    /* FULLWIDTH VERTICAL LINE */ DT_0453,
    /* FULLWIDTH RIGHT CURLY BRACKET */ DT_0454,
    /* FULLWIDTH TILDE */ DT_0455,
};
unichar const* const UnicharDecompositionData_HalfwidthKatakana[] =
{
    /* HALFWIDTH KATAKANA MIDDLE DOT */ DT_0456,
    /* HALFWIDTH KATAKANA LETTER WO */ DT_0360,
    /* HALFWIDTH KATAKANA LETTER SMALL A */ DT_0284,
    /* HALFWIDTH KATAKANA LETTER SMALL I */ DT_0285,
    /* HALFWIDTH KATAKANA LETTER SMALL U */ DT_0286,
    /* HALFWIDTH KATAKANA LETTER SMALL E */ DT_0287,
    /* HALFWIDTH KATAKANA LETTER SMALL O */ DT_0288,
    /* HALFWIDTH KATAKANA LETTER SMALL YA */ DT_0315,
    /* HALFWIDTH KATAKANA LETTER SMALL YU */ DT_0316,
    /* HALFWIDTH KATAKANA LETTER SMALL YO */ DT_0317,
    /* HALFWIDTH KATAKANA LETTER SMALL TU */ DT_0301,
    /* HALFWIDTH KATAKANA-HIRAGANA PROLONGED SOUND MARK */ DT_0457,
    /* HALFWIDTH KATAKANA LETTER A */ DT_0284,
    /* HALFWIDTH KATAKANA LETTER I */ DT_0285,
    /* HALFWIDTH KATAKANA LETTER U */ DT_0286,
    /* HALFWIDTH KATAKANA LETTER E */ DT_0287,
    /* HALFWIDTH KATAKANA LETTER O */ DT_0288,
    /* HALFWIDTH KATAKANA LETTER KA */ DT_0320,
    /* HALFWIDTH KATAKANA LETTER KI */ DT_0326,
    /* HALFWIDTH KATAKANA LETTER KU */ DT_0327,
    /* HALFWIDTH KATAKANA LETTER KE */ DT_0321,
    /* HALFWIDTH KATAKANA LETTER KO */ DT_0328,
    /* HALFWIDTH KATAKANA LETTER SA */ DT_0329,
    /* HALFWIDTH KATAKANA LETTER SI */ DT_0330,
    /* HALFWIDTH KATAKANA LETTER SU */ DT_0331,
    /* HALFWIDTH KATAKANA LETTER SE */ DT_0332,
    /* HALFWIDTH KATAKANA LETTER SO */ DT_0333,
    /* HALFWIDTH KATAKANA LETTER TA */ DT_0334,
    /* HALFWIDTH KATAKANA LETTER TI */ DT_0335,
    /* HALFWIDTH KATAKANA LETTER TU */ DT_0301,
    /* HALFWIDTH KATAKANA LETTER TE */ DT_0336,
    /* HALFWIDTH KATAKANA LETTER TO */ DT_0337,
    /* HALFWIDTH KATAKANA LETTER NA */ DT_0338,
    /* HALFWIDTH KATAKANA LETTER NI */ DT_0339,
    /* HALFWIDTH KATAKANA LETTER NU */ DT_0340,
    /* HALFWIDTH KATAKANA LETTER NE */ DT_0341,
    /* HALFWIDTH KATAKANA LETTER NO */ DT_0342,
    /* HALFWIDTH KATAKANA LETTER HA */ DT_0343,
    /* HALFWIDTH KATAKANA LETTER HI */ DT_0344,
    /* HALFWIDTH KATAKANA LETTER HU */ DT_0345,
    /* HALFWIDTH KATAKANA LETTER HE */ DT_0346,
    /* HALFWIDTH KATAKANA LETTER HO */ DT_0347,
    /* HALFWIDTH KATAKANA LETTER MA */ DT_0348,
    /* HALFWIDTH KATAKANA LETTER MI */ DT_0349,
    /* HALFWIDTH KATAKANA LETTER MU */ DT_0350,
    /* HALFWIDTH KATAKANA LETTER ME */ DT_0351,
    /* HALFWIDTH KATAKANA LETTER MO */ DT_0352,
    /* HALFWIDTH KATAKANA LETTER YA */ DT_0315,
    /* HALFWIDTH KATAKANA LETTER YU */ DT_0316,
    /* HALFWIDTH KATAKANA LETTER YO */ DT_0317,
    /* HALFWIDTH KATAKANA LETTER RA */ DT_0353,
    /* HALFWIDTH KATAKANA LETTER RI */ DT_0354,
    /* HALFWIDTH KATAKANA LETTER RU */ DT_0355,
    /* HALFWIDTH KATAKANA LETTER RE */ DT_0356,
    /* HALFWIDTH KATAKANA LETTER RO */ DT_0357,
    /* HALFWIDTH KATAKANA LETTER WA */ DT_0318,
    /* HALFWIDTH KATAKANA LETTER N */ DT_0361,
    /* HALFWIDTH KATAKANA VOICED SOUND MARK */ DT_0458,
    /* HALFWIDTH KATAKANA SEMI-VOICED SOUND MARK */ DT_0459,
};

const UnicodeDecompositionRange UnicharDecompositionData[] =
{
    {
        /*first*/ 0x00a0 /*NO-BREAK SPACE*/,
        /*last*/ 0x0233 /*LATIN SMALL LETTER Y WITH MACRON*/,
        /*data*/ UnicharDecompositionData_Latin
    },
    {
        /*first*/ 0x3041 /*HIRAGANA LETTER SMALL A*/,
        /*last*/ 0x30ff /*KATAKANA DIGRAPH KOTO*/,
        /*data*/ UnicharDecompositionData_Kana
    },
    {
        /*first*/ 0x31f0 /*KATAKANA LETTER SMALL KU*/,
        /*last*/ 0x31ff /*KATAKANA LETTER SMALL RO*/,
        /*data*/ UnicharDecompositionData_KatakanaPhoneticExtensions
    },
    {
        /*first*/ 0x32d0 /*CIRCLED KATAKANA A*/,
        /*last*/ 0x32fe /*CIRCLED KATAKANA WO*/,
        /*data*/ UnicharDecompositionData_CircledKatakana
    },
    {
        /*first*/ 0xff01 /*FULLWIDTH EXCLAMATION MARK*/,
        /*last*/ 0xff5e /*FULLWIDTH TILDE*/,
        /*data*/ UnicharDecompositionData_FullwidthLatin
    },
    {
        /*first*/ 0xff65 /*HALFWIDTH KATAKANA MIDDLE DOT*/,
        /*last*/ 0xff9f /*HALFWIDTH KATAKANA SEMI-VOICED SOUND MARK*/,
        /*data*/ UnicharDecompositionData_HalfwidthKatakana
    },
};

const unsigned UnicharDecompositionDataCount = ARRAY_SIZE(UnicharDecompositionData);
