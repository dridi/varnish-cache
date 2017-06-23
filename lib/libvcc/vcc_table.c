/*-
 * Copyright (c) 2016 Varnish Software
 * All rights reserved.
 *
 * Author: Dridi Boukelmoune <dridi.boukelmoune@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#include <string.h>

#include "vcc_compile.h"

#include "vqueue.h"
#include "vrt.h"

struct tbl_entry {
	VTAILQ_ENTRY(tbl_entry)	list;
	struct vrt_table_entry	entry;
};

VTAILQ_HEAD(tbl_head, tbl_entry);

static void
vcc_table_emit(struct vcc *tl, const char *name, unsigned len,
    struct tbl_head *head)
{
	struct tbl_entry *t;
	struct vsb *vsb;

	vsb = VSB_new_auto();

	Fh(tl, 0, "\nstatic const struct vrt_table %s = {\n", name);
	Fh(tl, 0, "\t.len = %u,\n", len);
	Fh(tl, 0, "\t.tbl = {\n");
	VTAILQ_FOREACH(t, head, list) {
		VSB_quote(vsb, t->entry.str, -1, VSB_QUOTE_CSTR);
		AZ(VSB_finish(vsb));
		Fh(tl, 0, "\t\t{%u,\t%s},\n", t->entry.len, VSB_data(vsb));
		VSB_clear(vsb);
	}
	Fh(tl, 0, "\t}\n");
	Fh(tl, 0, "};\n\n");

	VSB_destroy(&vsb);
}

static void
vcc_table_insert(struct vcc *tl, unsigned *len, struct tbl_head *head)
{
	struct tbl_entry *e, *t;
	int cmp;

	ExpectErr(tl, CSTR);
	e = TlAlloc(tl, sizeof *e);
	AN(e);
	e->entry.len = strlen(tl->t->dec);
	e->entry.str = tl->t->dec;

	VTAILQ_FOREACH(t, head, list) {
		cmp = e->entry.len - t->entry.len;
		if (cmp > 0)
			continue;
		if (cmp < 0)
			break;

		cmp = strcmp(e->entry.str, t->entry.str);
		if (cmp < 0)
			break;
		if (cmp == 0)
			return;
	}

	if (t == NULL)
		VTAILQ_INSERT_TAIL(head, e, list);
	else
		VTAILQ_INSERT_BEFORE(t, e, list);
	*len += 1;
}

void
vcc_ParseTable(struct vcc *tl)
{
	struct token *tn;
	struct symbol *sym;
	struct tbl_head head;
	unsigned len;

	vcc_NextToken(tl);
	vcc_ExpectVid(tl, "table");
	ERRCHK(tl);
	tn = tl->t;

	sym = VCC_HandleSymbol(tl, tn, TABLE, "&vtbl");
	ERRCHK(tl);
	AN(sym);

	VTAILQ_INIT(&head);

	vcc_NextToken(tl);
	SkipToken(tl, '{');
	len = 0;
	while (tl->t->tok != '}') {
		vcc_table_insert(tl, &len, &head);
		vcc_NextToken(tl);
		ERRCHK(tl);
		SkipToken(tl, ';');
	}
	SkipToken(tl, '}');

	/* NB: skip the '&' from rname. */
	vcc_table_emit(tl, sym->rname + 1, len, &head);
}
