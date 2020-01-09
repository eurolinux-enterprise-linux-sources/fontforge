typedef struct prefitem {
    unichar_t *name;
    void *data;		/* address of preference item, except for pt_tabs, when it's a bunch of struct prefitems */
    void *def_val;	/* unichar_t *, or int or float. NOT int * */
    enum pref_type { pt_int, pt_string, pt_url, pt_font, pt_boolean, pt_choice,
	    pt_orchoice, pt_list, pt_float, pt_color, pt_charset, pt_tab,
	    pt_line, pt_comment };
    int pt_min, pt_max;		/* 0,0 => unlimited */
    unichar_t **choices;
    struct prefitem *next;
} PrefItem, PrefPage;

void RegisterPrefPage(unichar_t *name, PrefPage *page);
/* At the top level (in the prefs module) there is an empty prefitem of type*/
/*  pt_tab. Modules will register their preference pages which adds name to */
/*  the top level choices field, and links page into the data field (and    */
/*  the old data goes into page->next) of the top level thing. page itself  */
/*  will just be an array of preference items, ending with a null data. */

void RegisterNestedPrefPage(unichar_t **names, PrefPage *page);
/* same as above if there is exactly one thing in names, otherwise:	      */
/*  start out at the top level and if the first name in it does not exist     */
/*	create it as a pt_tab itself.					      */
/*  then repeat, adding into the new item until there is only one name left   */
/*   then do as above.							      */
