#include "../../../users_groups/ugid_functions-c/ugid_functions.h"
#include "../../../lib/tlpi_hdr.h"
#include <sys/acl.h>
#include <acl/libacl.h>
#include <assert.h>

static void usage_error(char *);
static void dispaly_perm(const acl_permset_t *);
static void display_entry(const acl_entry_t *);

static void dispaly_perm(const acl_permset_t *perm)
{
	char str_perm[4] = { '-', '-', '-', '\0' };

	if (acl_get_perm(*perm, ACL_READ)) {
		str_perm[0] = 'r';
	}
	if (acl_get_perm(*perm, ACL_WRITE)) {
		str_perm[1] = 'w';
	}

	if (acl_get_perm(*perm, ACL_EXECUTE)) {
		str_perm[2] = 'x';
	}
	printf("     %s\n", str_perm);
}

static void display_entry(const acl_entry_t *entry)
{
	acl_tag_t tag_t;
	assert(acl_get_tag_type(*entry, &tag_t) == 0);
	switch (tag_t) {
	case ACL_USER_OBJ:
		printf("user_obj            ");
		break;
	case ACL_USER:
		printf("user      ");
		printf("%10s",
		       userNameFromId(*(uid_t *)acl_get_qualifier(*entry)));
		break;
	case ACL_GROUP_OBJ:
		printf("group_obj           ");
		break;
	case ACL_GROUP:
		printf("group     ");
		printf("%10s",
		       groupNameFromId(*(gid_t *)acl_get_qualifier(*entry)));
		break;
	case ACL_MASK:
		printf("mask                ");
		break;
	case ACL_OTHER:
		printf("other               ");
		break;
	default:
		fprintf(stderr, "unreachable!\n");
		exit(EXIT_FAILURE);
	}

	acl_permset_t permset;
	assert(acl_get_permset(*entry, &permset) == 0);
	dispaly_perm(&permset);
}

int main(int ac, char *av[])
{
	if (ac < 2) {
		usage_error(av[0]);
	}

	char *file = av[1];
	int default_acl = 0;
	if (strcmp(av[1], "-d") == 0) {
		file = av[2];
		default_acl = 1;
	}

	acl_t acl = acl_get_file(file, default_acl ? ACL_TYPE_DEFAULT :
						     ACL_TYPE_ACCESS);
	if (acl == NULL) {
		errExit("acl_get_file(%s) returns NULL", file);
	}

	acl_entry_t entry;
	int ret = acl_get_entry(acl, ACL_FIRST_ENTRY, &entry);
	if (ret == -1) {
		usage_error("acl_get_entry returns -1");
	} else if (ret == 0) {
		exit(0);
	}
	display_entry(&entry);

	while (acl_get_entry(acl, ACL_NEXT_ENTRY, &entry) == 1) {
		display_entry(&entry);
	}

	acl_free(acl);
}

static void usage_error(char *prog_name)
{
	fprintf(stderr, "Usage: %s [-d] filename\n", prog_name);
	exit(EXIT_FAILURE);
}