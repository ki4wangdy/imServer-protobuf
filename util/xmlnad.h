
#ifndef _UTIL_XML_H_
#define _UTIL_XML_H_

struct nad_elem_st {
	int parent;
	int iname, lname;
	int icdata, lcdata; /* cdata within this elem (up to first child) */
	int itail, ltail; /* cdata after this elem */
	int attr;
	int ns;
	int my_ns;
	int depth;
};

struct nad_attr_st {
	int iname, lname;
	int ival, lval;
	int my_ns;
	int next;
};

struct nad_ns_st {
	int iuri, luri;
	int iprefix, lprefix;
	int next;
};

typedef struct nad_st
{
	struct nad_elem_st *elems;
	struct nad_attr_st *attrs;
	struct nad_ns_st *nss;
	char *cdata;
	int *depths; /* for tracking the last elem at a depth */

	/* The size in bytes of the elems, attrs, nss and cdata buffers, respectively. */
	int elen, alen, nlen, clen, dlen;

	/* The number of elements of each type of that data that are actually stored in the elems, attrs, nss and cdata buffers, respectively. */
	int ecur, acur, ncur, ccur;

	int scope; /* currently scoped namespaces, get attached to the next element */
	struct nad_st *next; /* for keeping a list of nads */
} *nad_t;

/** create a new nad */
nad_t nad_new(void);

/** copy a nad */
nad_t nad_copy(nad_t nad);

/** free that nad */
void nad_free(nad_t nad);

/** find the next element with this name/depth */
/** 0 for siblings, 1 for children and so on */
int nad_find_elem(nad_t nad, int elem, int ns, const char *name, int depth);

/** find the first matching attribute (and optionally value) */
int nad_find_attr(nad_t nad, int elem, int ns, const char *name, const char *val);

/** find the first matching namespace (and optionally prefix) */
int nad_find_namespace(nad_t nad, int elem, const char *uri, const char *prefix);

/** find a namespace in scope (and optionally prefix) */
int nad_find_scoped_namespace(nad_t nad, const char *uri, const char *prefix);

/** find elem using XPath like query
*  name -- "name" for the child tag of that name
*          "name/name" for a sub child (recurses)
*          "?attrib" to match the first tag with that attrib defined
*          "?attrib=value" to match the first tag with that attrib and value
*          or any combination: "name/name/?attrib", etc
*/
int nad_find_elem_path(nad_t nad, int elem, int ns, const char *name);

/** reset or store the given attribute */
void nad_set_attr(nad_t nad, int elem, int ns, const char *name, const char *val, int vallen);

/** insert and return a new element as a child of this one */
int nad_insert_elem(nad_t nad, int elem, int ns, const char *name, const char *cdata);

/** remove an element (and its subelements) */
void nad_drop_elem(nad_t nad, int elem);

/** wrap an element with another element */
void nad_wrap_elem(nad_t nad, int elem, int ns, const char *name);

/** insert part of a nad into another nad */
int nad_insert_nad(nad_t dest, int delem, nad_t src, int selem);

/** append and return a new element */
int nad_append_elem(nad_t nad, int ns, const char *name, int depth);

/** append attribs to the last element */
int nad_append_attr(nad_t nad, int ns, const char *name, const char *val);

/** append more cdata to the last element */
void nad_append_cdata(nad_t nad, const char *cdata, int len, int depth);

/** add a namespace to the next element (ie, called when the namespace comes into scope) */
int nad_add_namespace(nad_t nad, const char *uri, const char *prefix);

/** declare a namespace on an already existing element */
int nad_append_namespace(nad_t nad, int elem, const char *uri, const char *prefix);

/** create a string representation of the given element (and children), point references to it */
void nad_print(nad_t nad, int elem, const char **xml, int *len);

/** serialize and deserialize a nad */
void nad_serialize(nad_t nad, char **buf, int *len);
nad_t nad_deserialize(const char *buf);

/** create a nad from raw xml */
nad_t nad_parse(const char *buf, int len);

/* these are some helpful macros */
#define NAD_ENAME(N,E) (N->cdata + N->elems[E].iname)
#define NAD_ENAME_L(N,E) (N->elems[E].lname)
#define NAD_CDATA(N,E) (N->cdata + N->elems[E].icdata)
#define NAD_CDATA_L(N,E) (N->elems[E].lcdata)
#define NAD_ANAME(N,A) (N->cdata + N->attrs[A].iname)
#define NAD_ANAME_L(N,A) (N->attrs[A].lname)
#define NAD_AVAL(N,A) (N->cdata + N->attrs[A].ival)
#define NAD_AVAL_L(N,A) (N->attrs[A].lval)
#define NAD_NURI(N,NS) (N->cdata + N->nss[NS].iuri)
#define NAD_NURI_L(N,NS) (N->nss[NS].luri)
#define NAD_NPREFIX(N,NS) (N->cdata + N->nss[NS].iprefix)
#define NAD_NPREFIX_L(N,NS) (N->nss[NS].lprefix)

#define NAD_ENS(N,E) (N->elems[E].my_ns)
#define NAD_ANS(N,A) (N->attrs[A].my_ns)

#define uri_XML         "http://www.w3.org/XML/1998/namespace"

typedef nad_t util_xml_t;

#define util_xml_new			nad_new
#define util_xml_copy			nad_copy
#define util_xml_free			nad_free

#define util_xml_find_elem		nad_find_elem
#define util_xml_find_attr		nad_find_attr

#define util_xml_find_namespace			nad_find_namespace
#define util_xml_find_scoped_namespace	nad_find_scoped_namespace

#define util_xml_find_elem_path		nad_find_elem_path
#define util_xml_set_attr			nad_set_attr

#define util_xml_insert_elem		nad_insert_elem
#define util_xml_drop_elem			nad_drop_elem

#define util_xml_wrap_elem			nad_wrap_elem
#define util_xml_insert_nad			nad_insert_nad

#define util_xml_append_elem		nad_append_elem
#define util_xml_append_attr		nad_append_attr

#define util_xml_append_cdata		nad_append_cdata
#define util_xml_add_namespace		nad_add_namespace

#define util_xml_append_namespace   nad_append_namespace
#define util_xml_print				nad_print

#define util_xml_serialize			nad_serialize
#define util_xml_deserialize		nad_deserialize

#endif