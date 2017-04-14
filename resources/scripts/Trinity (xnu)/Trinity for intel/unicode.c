/*
 * Routines for generating a page of mangled unicode.
 *
 * Inspiration:
 *  http://www.cl.cam.ac.uk/~mgk25/ucs/examples/quickbrown.txt
 *  http://www.columbia.edu/~fdc/utf8/
 *  http://www.cl.cam.ac.uk/~mgk25/unicode.html
 *  http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
 *  http://stackoverflow.com/questions/1319022/really-good-bad-utf-8-example-test-data
 *  http://www.twitter.com/glitchr
 *
 * Lots more to do here, but this is a start.
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "arch.h"
#include "sanitise.h"

void gen_unicode_page(char *page)
{
	unsigned int i = 0, j, l;
	unsigned int unilen;

	const char unicode1[4] = { 0xb8, 0xe0, 0xe0, 0xaa };
	const char unicode2[6] = { 0x89, 0xb9, 0xb9, 0xe0, 0xe0, 0x89 };
	const char unicode3[2] = { 0x89, 0xb9 };
	const char unicode4[18] = { 0xbb, 0xef, 0xd2, 0xa9, 0xd2, 0x88, 0x20, 0x88, 0x88, 0xd2, 0x88, 0xd2, 0xd2, 0x20, 0xd2, 0x88, 0x0a, 0x88 };
	const char unicode5[4] = { 0xd9, 0x20, 0xd2, 0x87 };
	const char unicode6[4] = { 0xcc, 0x88, 0xd2, 0xbf };
	const char unicode7[2] = { 0x0a, 0xbf };
	const char *zalgo[] = { "T̫̺̳o̬̜ ì̬͎̲̟nv̖̗̻̣̹̕o͖̗̠̜̤k͍͚̹͖̼e̦̗̪͍̪͍ ̬ͅt̕h̠͙̮͕͓e̱̜̗͙̭ ̥͔̫͙̪͍̣͝ḥi̼̦͈̼v҉̩̟͚̞͎e͈̟̻͙̦̤-m̷̘̝̱í͚̞̦̳n̝̲̯̙̮͞d̴̺̦͕̫ ̗̭̘͎͖r̞͎̜̜͖͎̫͢ep͇r̝̯̝͖͉͎̺e̴s̥e̵̖̳͉͍̩̗n̢͓̪͕̜̰̠̦t̺̞̰i͟n҉̮̦̖̟g̮͍̱̻͍̜̳ ̳c̖̮̙̣̰̠̩h̷̗͍̖͙̭͇͈a̧͎̯̹̲̺̫ó̭̞̜̣̯͕s̶̤̮̩̘.̨̻̪̖͔  ̳̭̦̭̭̦̞́I̠͍̮n͇̹̪̬v̴͖̭̗̖o̸k҉̬̤͓͚̠͍i͜n̛̩̹͉̘̹g͙ ̠̥ͅt̰͖͞h̫̼̪e̟̩̝ ̭̠̲̫͔fe̤͇̝̱e͖̮̠̹̭͖͕l͖̲̘͖̠̪i̢̖͎̮̗̯͓̩n̸̰g̙̱̘̗͚̬ͅ ͍o͍͍̩̮͢f̖͓̦̥ ̘͘c̵̫̱̗͚͓̦h͝a̝͍͍̳̣͖͉o͙̟s̤̞.̙̝̭̣̳̼͟  ̢̻͖͓̬̞̰̦W̮̲̝̼̩̝͖i͖͖͡ͅt̘̯͘h̷̬̖̞̙̰̭̳ ̭̪̕o̥̤̺̝̼̰̯͟ṳ̞̭̤t̨͚̥̗ ̟̺̫̩̤̳̩o̟̰̩̖ͅr̞̘̫̩̼d̡͍̬͎̪̺͚͔e͓͖̝̙r̰͖̲̲̻̠.̺̝̺̟͈  ̣̭T̪̩̼h̥̫̪͔̀e̫̯͜ ̨N̟e҉͔̤zp̮̭͈̟é͉͈ṛ̹̜̺̭͕d̺̪̜͇͓i̞á͕̹̣̻n͉͘ ̗͔̭͡h̲͖̣̺̺i͔̣̖̤͎̯v̠̯̘͖̭̱̯e̡̥͕-m͖̭̣̬̦͈i͖n̞̩͕̟̼̺͜d̘͉ ̯o̷͇̹͕̦f̰̱ ̝͓͉̱̪̪c͈̲̜̺h̘͚a̞͔̭̰̯̗̝o̙͍s͍͇̱͓.̵͕̰͙͈ͅ ̯̞͈̞̱̖Z̯̮̺̤̥̪̕a͏̺̗̼̬̗ḻg͢o̥̱̼.̺̜͇͡ͅ ̴͓͖̭̩͎̗  ̧̪͈̱̹̳͖͙H̵̰̤̰͕̖e̛ ͚͉̗̼̞w̶̩̥͉̮h̩̺̪̩͘ͅọ͎͉̟ ̜̩͔̦̘ͅW̪̫̩̣̲͔̳a͏͔̳͖i͖͜t͓̤̠͓͙s̘̰̩̥̙̝ͅ ̲̠̬̥Be̡̙̫̦h̰̩i̛̫͙͔̭̤̗̲n̳͞d̸ ͎̻͘T̛͇̝̲̹̠̗ͅh̫̦̝ͅe̩̫͟ ͓͖̼W͕̳͎͚̙̥ą̙l̘͚̺͔͞ͅl̳͍̙̤̤̮̳.̢  ̟̺̜̙͉Z̤̲̙̙͎̥̝A͎̣͔̙͘L̥̻̗̳̻̳̳͢G͉̖̯͓̞̩̦O̹̹̺!̙͈͎̞̬ T̷̗͑̃ͦ̊͊̓͡͞h̛̥͚̖͇̝̬̹̟̃̋̽ͥ͆̂ͦ͗ȅ̱͗͘ ̱͈͋͛͡͝e͙͓̼̰̹̲̒ͤ́ͫ̓́͊͆͢n͂͗͊͌ͦ̐̊҉̧̟̺t̐ͧ̍̀ͤ̈́̔̒͏̻̜͙̝͕͍ͅï̛̠̩̦̿̉ͪ͌ͩ̚r̶̳̺͈̺͈͚͍ͯ̄ͤ͌̋͂͝e͖̟͓̝̩̺̬̭͙̐̉ ̵̴̴̗̤̺̥̰͚̐ͧͥͦ̿͛ͤr͙̣͔̺͆͞o̶̠̯͇͈ͧͭ̄ͩ̔̍͞ơ̖͙͔̥͋ͣ̈m͙͉̱̱̞̦̘̰͑ͭ͒̂̀͘ ̛̔ͭ̈ͬͮ҉̞͢ͅiͮ̒̔͘͏͉̯͎̦s̶̡͈͎̱̖͎̫̙̊ͫ̿̋ͣ͡ͅ ̙̣̬̟͈͋̓f͍͈̰̘̻̃͋͂̐i̸̺͙̪͔̞͙̣ͧͨ͝lͬ͂҉̸̻͔̙l͖̼ͮ͢͝e̷̟̙͔̠̯̓̓ͧ́ͬͪ̇̃d̶͕͇͎̦̐̓ ̮̯̥́͋̚̕w͑̍̔̔̀ͪͯ͘͢͏͉̰i̛̟̰̣ͭ̌̊͑̒ͫ̉͠t͇̖̬̠̗̲̄̓̈ͣͮ̂͂̊͗ḩ̲̖̊ͪ̓̄ ̝̺̟͕͖̈͋̎ͩͩ͆̈́̿͡Z̵̴̖͖͕̔ͬͮ͒̏̅̍̎a̤͖ͬ͑̎͐͑̔ͭ͞l̝̼̩͋̂g̨ͦ̓̓̏̈́̉ͯ͏͎͔̟̮̠̬͙ỏ͍̝̺͕͈ͭ̓̏̽̓ͭ̾́͢͞.͓̹͇̬̔̓̏ͦ̚͜͠" };
	const char *thai[] = { "ก็็็็็็็็็็กิิิิิิิิิิก้้้้้้้้้้ก็็็็็็็็็็ก็็็็็็็็็็กิิิิิิิิิิก้้้้้้้้้้" };

	unsigned int zalgolen = strlen(*zalgo);
	unsigned int thailen = strlen(*thai);

	char *ptr = page;

	while (i < (page_size - zalgolen)) {

		j = rand() % 9;

		switch (j) {

		case 0:
			strncpy(ptr, unicode1, 4);
			ptr += 4;
			i += 4;
			break;

		case 1: unilen = rand() % 10;
			for (l = 0; l < unilen; l++) {
				strncpy(ptr, unicode2, 6);
				ptr += 6;
				i += 6;
				if ((i + 6) > page_size)
					break;
			}
			break;

		case 2:	strncpy(ptr, unicode3, 2);
			i += 2;
			ptr += 2;
			break;
		case 3:	strncpy(ptr, unicode4, 18);
			i += 18;
			ptr += 18;
			break;

		case 4:	strncpy(ptr, unicode5, 4);
			i += 4;
			ptr += 4;
			break;

		case 5: unilen = rand() % 10;
			for (l = 0; l < unilen; l++) {
				strncpy(ptr, unicode6, 4);
				ptr += 4;
				i += 4;
				if ((i + 4) > page_size)
					break;
			}
			break;

		case 6:	strncpy(ptr, unicode7, 4);
			i += 4;
			ptr += 4;
			break;

		/* HE COMES. */
		case 7:	strncpy(ptr, *zalgo, zalgolen);
			i += zalgolen;
			ptr += zalgolen;
			break;

		case 8:	strncpy(ptr, *thai, thailen);
			i += thailen;
			ptr += thailen;
			break;

		default:
			break;
		}
	}

	page[rand() % page_size] = 0;
}

#ifdef STANDALONE
/*
 * gcc -I include -g -DSTANDALONE unicode.c -o unicode
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int page_size = 4096;

void main(int argc, char* argv[])
{
	unsigned char *page;
	unsigned int x = 0, y, n = 0;
	struct timeval t;

	gettimeofday(&t, 0);
	srand((t.tv_sec * getpid()) ^ t.tv_usec);

	page = malloc(4096);
	memset(page, 0, 4096);

	gen_unicode_page(page);

	for (y = 0; y < 4096; y+=32) {
		for (x = 0; x < 32; x++) {
			printf("%c", page[n++]);
		}
	}
}
#endif
