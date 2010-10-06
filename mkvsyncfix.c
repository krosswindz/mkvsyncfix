/*
 * Copyright (c) 2009 Kross Windz <krosswindz@gmail.com>.
 * All rights reserved.
 */

#include <curses.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "cmdopt.h"
#include "libiconv.h"
#include "libintl.h"
#include "magic1.h"
#include "mkvmerge.h"
#include "regex2.h"
#include "zlib1.h"

#define VER_MAJ 0
#define VER_MIN 1

static inline int create_files (void);
static inline void delete_files (void);
static inline int gen_file_name (void);

static char *libiconv, *libintl, *magic, *mkvmerge, *regx, *zlib;

int
main (void)
{
	char *cmd = NULL;
	size_t cmd_len = 2;
	int ret;
	WINDOW *win = NULL;

	if (gen_file_name () != 0)
		return EXIT_FAILURE;

	if (create_files () != 0)
		return EXIT_FAILURE;

	cmd_len += strlen (mkvmerge) + strlen (opt);
	cmd = (char *) malloc (sizeof (char) * cmd_len);
	if (cmd == NULL)
		goto exit;

	sprintf (cmd, "%s %s", mkvmerge, opt);
	initscr ();
	cbreak ();
	noecho ();
	refresh ();
	win = newwin (24, 80, 0, 0);
	if (win == NULL)
		goto exit;

	werase (win);
	wprintw (win, "TmG mkvsyncfix v%d.%d\n", VER_MAJ, VER_MIN);
	wprintw (win, "Copyright (c) 2010 Kross Windz "
			"<krosswindz@gmail.com>\n");
	wprintw (win, "All rights reserved.\n");
	wprintw (win, "\nSyncing %s...\n", rls);
	wrefresh (win);
	ret = system (cmd);
	if (ret > 1) {
		wclear (win);
		wprintw (win, "TmG mkvsyncfix v%d.%d\n", VER_MAJ, VER_MIN);
		wprintw (win, "Copyright (c) 2010 Kross Windz "
				"<krosswindz@gmail.com>\n");
		wprintw (win, "All rights reserved.\n");
		wprintw (win, "\nSyncing %s... failed\n", rls);
		wrefresh (win);
		Sleep (5000);
		delwin (win);
		goto exit;
	}

	wclear (win);
	wprintw (win, "TmG mkvsyncfix v%d.%d\n", VER_MAJ, VER_MIN);
	wprintw (win, "Copyright (c) 2010 Kross Windz "
			"<krosswindz@gmail.com>\n");
	wprintw (win, "All rights reserved.\n");
	wprintw (win, "\nSyncing %s... done.", rls);
	wrefresh (win);
	Sleep (5000);
	delwin (win);
exit:
	endwin ();
	delete_files ();

	return EXIT_SUCCESS;
}

static inline int
create_files (void)
{
	FILE *fp;
	uint64_t i;

	fp = fopen (libiconv, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open libiconv-2.dll for "
				"writing.\n");
		return -1;
	}

	for (i = 0; i < liconv_size; i++) {
		if (fwrite (&liconv[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to "
					"libiconv-2.dll.\n");
			return -1;
		}
	}

	fclose (fp);
	fp = fopen (libintl, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open libintl-8.dll for "
				"writing.\n");
		return -1;
	}

	for (i = 0; i < lintl_size; i++) {
		if (fwrite (&lintl[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to "
					"libintl-8.dll.\n");
			return -1;
		}
	}

	fclose (fp);
	fp = fopen (magic, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open magic1.dll for writing.\n");
		return -1;
	}

	for (i = 0; i < magic1_size; i++) {
		if (fwrite (&magic1[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to magic1.dll.\n");
			return -1;
		}
	}

	fclose (fp);
	fp = fopen (mkvmerge, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open mkvmerge.exe for writing.\n");
		return -1;
	}

	for (i = 0; i < mkvme_size; i++) {
		if (fwrite (&mkvme[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to mkvmerge.exe.\n");
			return -1;
		}
	}

	fclose (fp);
	fp = fopen (regx, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open regex2.dll for writing.\n");
		return -1;
	}

	for (i = 0; i < regex2_size; i++) {
		if (fwrite (&regex2[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to regex2.dll\n");
			return -1;
		}
	}

	fclose (fp);
	fp = fopen (zlib, "wb");
	if (fp == NULL) {
		fprintf (stderr, "Unable to open zlib1.dll for writing.\n");
		return -1;
	}

	for (i = 0; i < zlib1_size; i++) {
		if (fwrite (&zlib1[i], sizeof (uint8_t), 1, fp)
				< sizeof (uint8_t)) {
			fprintf (stderr, "Failed to write to zlib1.dll\n");
			return -1;
		}
	}

	fclose (fp);

	return 0;
}

static inline void
delete_files (void)
{
	if (libiconv != NULL) {
		remove (libiconv);
		free (libiconv);
	}

	if (libintl != NULL) {
		remove (libintl);
		free (libintl);
	}

	if (mkvmerge != NULL) {
		remove (mkvmerge);
		free (mkvmerge);
	}

	if (magic != NULL) {
		remove (magic);
		free (magic);
	}

	if (regx != NULL) {
		remove (regx);
		free (regx);
	}

	if (zlib != NULL) {
		remove (zlib);
		free (zlib);
	}

	return;
}

static inline int
gen_file_name (void)
{
	char *tmpdir;

	tmpdir = getenv ("TMP");
	if (tmpdir == NULL)
		tmpdir = getenv ("TEMP");

	if (tmpdir == NULL) {
		fprintf (stderr, "No temp directory defined.\n");
		return -1;
	}

	libiconv = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 16));
	if (libiconv == NULL) {
		fprintf (stderr, "Unable to allocate libiconv-2.dll path.\n");
		return -1;
	}

	sprintf (libiconv, "%s\\libiconv-2.dll", tmpdir);
	libintl = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 15));
	if (libintl == NULL) {
		fprintf (stderr, "Unable to allocate libintl-8.dll path.\n");
		return -1;
	}

	sprintf (libintl, "%s\\libintl-8.dll", tmpdir);
	magic = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 12));
	if (magic == NULL) {
		fprintf (stderr, "Unable to allocate magic1.dll path.\n");
		return -1;
	}

	sprintf (magic, "%s\\magic1.dll", tmpdir);
	mkvmerge = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 14));
	if (mkvmerge == NULL) {
		fprintf (stderr, "Unable to allocate mkvmerge.exe path.\n");
		return -1;
	}

	sprintf (mkvmerge, "%s\\mkvmerge.exe", tmpdir);
	regx = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 12));
	if (regx == NULL) {
		fprintf (stderr, "Unable to allocate regex2.dll path.\n");
		return -1;
	}

	sprintf (regx, "%s\\regex2.dll", tmpdir);
	zlib = (char *) malloc (sizeof (char) * (strlen (tmpdir) + 11));
	if (zlib == NULL) {
		fprintf (stderr, "Unable to allocate zlib1.dll path.\n");
		return -1;
	}

	sprintf (zlib, "%s\\zlib1.dll", tmpdir);

	return 0;
}
