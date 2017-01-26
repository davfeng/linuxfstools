
//arm-linux-gnueabi-gcc -o inodes_arm -static -D_FILE_OFFSET_BITS=64 inodes.c
//gcc -o inodes -static -D_FILE_OFFSET_BITS=64 inodes.c

#define _INO_T_DEFINED 1

#define HAVE_STDLIB_H  1
#define HAVE_STRING_H  1
#define HAVE_GETOPT_H  1
#define HAVE_ERRNO_H   1
#define HAVE_SETJMP_H  1
#define HAVE_STRCASECMP 1
#define HAVE_SRANDOM    1
#define CONFIG_MMP     1
#define HAVE_CONIO_H   1
#define HAVE_EXT2_INODE_VERSION 1
//#define HAVE_FSTAT64 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_IOCTL_H 1
/* Define to 1 if you have the `pwrite' function. */
#define HAVE_PWRITE 1
#define HAVE_LSEEK64 1
#define HAVE_LSEEK64_PROTOTYPE 1
/* Define to 1 if you have the `pwrite64' function. */
#define HAVE_PWRITE64 1
#define HAVE_MEMALIGN 1
#define HAVE_VALLOC 1

#define ENABLE_NLS 1
/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `off_t', as computed by sizeof. */
#define SIZEOF_OFF_T 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2


//#define inline __forceinline

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#include <sys/types.h>

#ifdef __linux__
#include <sys/utsname.h>
#endif
#include <sys/types.h>
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#include <sys/stat.h>
#if HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#if HAVE_LINUX_FALLOC_H
#include <linux/falloc.h>
#endif
#include <fcntl.h>

#define _(a) (gettext (a))

typedef unsigned char __u8;
typedef signed char   __s8;
typedef int64_t  __s64;
typedef long long unsigned int __u64;
typedef signed   short    __s16;
typedef unsigned short    __u16;
typedef signed   int    __s32;
typedef unsigned int    __u32;

#ifdef __CHECKER__
#define __bitwise               __attribute__((bitwise))
#define __force                 __attribute__((force))
#else
#define __bitwise
#define __force
#endif

#define _INLINE_

typedef __u16   __bitwise       __le16;
typedef __u32   __bitwise       __le32;
typedef __u64   __bitwise       __le64;
typedef __u16   __bitwise       __be16;
typedef __u32   __bitwise       __be32;
typedef __u64   __bitwise       __be64;

typedef __u32 	ext2_ino_t;
typedef __u32 	blk_t;
typedef __u64 	blk64_t;
typedef __u32 	dgrp_t;
typedef __u32 	ext2_off_t;
typedef __u64 	ext2_off64_t;
typedef __s64 	e2_blkcnt_t;
typedef __u32 	ext2_dirhash_t;

typedef long errcode_t;

typedef off_t ext2_loff_t;

#ifdef __STDC__
#define NOARGS void
#else
#define NOARGS
#define const
#endif

#ifdef __GNUC__
#define EXT2FS_ATTR(x) __attribute__(x)
#else
#define EXT2FS_ATTR(x)
#endif

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#if (__GNUC__ >= 4)
#define ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#else
#define ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef __GNUC__
#define COM_ERR_ATTR(x) __attribute__(x)
#else
#define COM_ERR_ATTR(x)
#endif

#include <stddef.h>
#include <stdarg.h>

typedef long errcode_t;

extern void com_err (const char *, long, const char *, ...)
	COM_ERR_ATTR((format(printf, 3, 4)));

extern void com_err_va (const char *whoami, errcode_t code, const char *fmt,
		 va_list args)
	COM_ERR_ATTR((format(printf, 3, 0)));

#define	ERRCODE_RANGE	8	/* # of bits to shift table number */
#define	BITS_PER_CHAR	6	/* # bits to shift per character in name */

static void
default_com_err_proc (const char *whoami, errcode_t code, const
		      char *fmt, va_list args)
	COM_ERR_ATTR((format(printf, 3, 0)));

static void
default_com_err_proc (const char *whoami, errcode_t code, const
		      char *fmt, va_list args)
{
    int do_cr = 1, fd = fileno(stderr);

    if (whoami) {
	fputs(whoami, stderr);
	fputs(": ", stderr);
    }
    if (code) {
	fputs(code, stderr);
	fputs(" ", stderr);
    }
    if (fmt) {
        vfprintf (stderr, fmt, args);
    }
    if (!isatty(fd))
	do_cr = 0;
#ifdef HAVE_TERMIOS_H
    else {
	struct termios t;

	if ((tcgetattr(fd, &t)) == 0 &&
	    (t.c_oflag & OPOST) && (t.c_oflag & ONLCR))
	do_cr = 0;
    }
#endif
    if (do_cr)
	fputc('\r', stderr);
    fputc('\n', stderr);
    fflush(stderr);
}

typedef void (*errf) (const char *, errcode_t, const char *, va_list);

errf com_err_hook = default_com_err_proc;

void com_err_va (const char *whoami, errcode_t code, const char *fmt,
		 va_list args)
{
    (*com_err_hook) (whoami, code, fmt, args);
}

void com_err (const char *whoami,
	      errcode_t code,
	      const char *fmt, ...)
{
    va_list pvar;

    if (!com_err_hook)
	com_err_hook = default_com_err_proc;
    va_start(pvar, fmt);
    com_err_va (whoami, code, fmt, pvar);
    va_end(pvar);
}

/* How many bits at a time to use.  Valid values are 1, 2, 4, 8, 32 and 64. */
/* For less performance-sensitive, use 4 */
#ifndef CRC_LE_BITS
# define CRC_LE_BITS 64
#endif
#ifndef CRC_BE_BITS
# define CRC_BE_BITS 64
#endif

#ifdef WORDS_BIGENDIAN
#define __constant_cpu_to_le32(x) ___constant_swab32((x))
#define __constant_cpu_to_be32(x) (x)
#define __be32_to_cpu(x) (x)
#define __cpu_to_be32(x) (x)
#define __cpu_to_le32(x) (ext2fs_cpu_to_le32((x)))
#define __le32_to_cpu(x) (ext2fs_le32_to_cpu((x)))
#else
#define __constant_cpu_to_le32(x) (x)
#define __constant_cpu_to_be32(x) ___constant_swab32((x))
#define __be32_to_cpu(x) (ext2fs_be32_to_cpu((x)))
#define __cpu_to_be32(x) (ext2fs_cpu_to_be32((x)))
#define __cpu_to_le32(x) (x)
#define __le32_to_cpu(x) (x)
#endif

#if CRC_LE_BITS > 8
# define tole(x) __constant_cpu_to_le32(x)
#else
# define tole(x) (x)
#endif

#if CRC_BE_BITS > 8
# define tobe(x) __constant_cpu_to_be32(x)
#else
# define tobe(x) (x)
#endif

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#if CRC_LE_BITS > 8 || CRC_BE_BITS > 8

/* implements slicing-by-4 or slicing-by-8 algorithm */
static inline uint32_t
crc32_body(uint32_t crc, unsigned char const *buf, size_t len,
	   const uint32_t (*tab)[256])
{
# ifndef WORDS_BIGENDIAN
#  define DO_CRC(x) (crc = t0[(crc ^ (x)) & 255] ^ (crc >> 8))
#  define DO_CRC4 (t3[(q) & 255] ^ t2[(q >> 8) & 255] ^ \
		   t1[(q >> 16) & 255] ^ t0[(q >> 24) & 255])
#  define DO_CRC8 (t7[(q) & 255] ^ t6[(q >> 8) & 255] ^ \
		   t5[(q >> 16) & 255] ^ t4[(q >> 24) & 255])
# else
#  define DO_CRC(x) (crc = t0[((crc >> 24) ^ (x)) & 255] ^ (crc << 8))
#  define DO_CRC4 (t0[(q) & 255] ^ t1[(q >> 8) & 255] ^ \
		   t2[(q >> 16) & 255] ^ t3[(q >> 24) & 255])
#  define DO_CRC8 (t4[(q) & 255] ^ t5[(q >> 8) & 255] ^ \
		   t6[(q >> 16) & 255] ^ t7[(q >> 24) & 255])
# endif
	const uint32_t *b;
	size_t rem_len;
	const uint32_t *t0 = tab[0], *t1 = tab[1], *t2 = tab[2], *t3 = tab[3];
	const uint32_t *t4 = tab[4], *t5 = tab[5], *t6 = tab[6], *t7 = tab[7];
	uint32_t q;

	/* Align it */
	if (((long)buf & 3 && len)) {
		do {
			DO_CRC(*buf++);
		} while ((--len) && ((long)buf)&3);
	}

# if CRC_LE_BITS == 32
	rem_len = len & 3;
	len = len >> 2;
# else
	rem_len = len & 7;
	len = len >> 3;
# endif

	b = (const uint32_t *)buf;
	for (--b; len; --len) {
		q = crc ^ *++b; /* use pre increment for speed */
# if CRC_LE_BITS == 32
		crc = DO_CRC4;
# else
		crc = DO_CRC8;
		q = *++b;
		crc ^= DO_CRC4;
# endif
	}
	len = rem_len;
	/* And the last few bytes */
	if (len) {
		const uint8_t *p = (const uint8_t *)(b + 1) - 1;
		do {
			DO_CRC(*++p); /* use pre increment for speed */
		} while (--len);
	}
	return crc;
#undef DO_CRC
#undef DO_CRC4
#undef DO_CRC8
}
#endif

/**
 * crc32_le() - Calculate bitwise little-endian Ethernet AUTODIN II CRC32
 * @crc: seed value for computation.  ~0 for Ethernet, sometimes 0 for
 *	other uses, or the previous crc32 value if computing incrementally.
 * @p: pointer to buffer over which CRC is run
 * @len: length of buffer @p
 */
static inline uint32_t crc32_le_generic(uint32_t crc, unsigned char const *p,
					size_t len, const uint32_t (*tab)[256],
					uint32_t polynomial EXT2FS_ATTR((unused)))
{
#if CRC_LE_BITS == 1
	int i;
	while (len--) {
		crc ^= *p++;
		for (i = 0; i < 8; i++)
			crc = (crc >> 1) ^ ((crc & 1) ? polynomial : 0);
	}
# elif CRC_LE_BITS == 2
	while (len--) {
		crc ^= *p++;
		crc = (crc >> 2) ^ tab[0][crc & 3];
		crc = (crc >> 2) ^ tab[0][crc & 3];
		crc = (crc >> 2) ^ tab[0][crc & 3];
		crc = (crc >> 2) ^ tab[0][crc & 3];
	}
# elif CRC_LE_BITS == 4
	while (len--) {
		crc ^= *p++;
		crc = (crc >> 4) ^ tab[0][crc & 15];
		crc = (crc >> 4) ^ tab[0][crc & 15];
	}
# elif CRC_LE_BITS == 8
	/* aka Sarwate algorithm */
	while (len--) {
		crc ^= *p++;
		crc = (crc >> 8) ^ tab[0][crc & 255];
	}
# else
	crc = __cpu_to_le32(crc);
	crc = crc32_body(crc, p, len, tab);
	crc = __le32_to_cpu(crc);
#endif
	return crc;
}

#define CRC32C_POLY_LE 0x82F63B78

#define ENTRIES_PER_LINE 4

#if CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#else
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

static uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

uint32_t ext2fs_crc32c_le(uint32_t crc, unsigned char const *p, size_t len)
{
	return crc32_le_generic(crc, p, len, crc32ctable_le, CRC32C_POLY_LE);
}

#define EXT2_ET_BASE                             (2133571328L)
#define EXT2_ET_MAGIC_EXT2FS_FILSYS              (2133571329L)
#define EXT2_ET_MAGIC_BADBLOCKS_LIST             (2133571330L)
#define EXT2_ET_MAGIC_BADBLOCKS_ITERATE          (2133571331L)
#define EXT2_ET_MAGIC_INODE_SCAN                 (2133571332L)
#define EXT2_ET_MAGIC_IO_CHANNEL                 (2133571333L)
#define EXT2_ET_MAGIC_UNIX_IO_CHANNEL            (2133571334L)
#define EXT2_ET_MAGIC_IO_MANAGER                 (2133571335L)
#define EXT2_ET_MAGIC_BLOCK_BITMAP               (2133571336L)
#define EXT2_ET_MAGIC_INODE_BITMAP               (2133571337L)
#define EXT2_ET_MAGIC_GENERIC_BITMAP             (2133571338L)
#define EXT2_ET_MAGIC_TEST_IO_CHANNEL            (2133571339L)
#define EXT2_ET_MAGIC_DBLIST                     (2133571340L)
#define EXT2_ET_MAGIC_EXTENT_HANDLE              (2133571346L)
#define EXT2_ET_BAD_MAGIC                        (2133571347L)
#define EXT2_ET_REV_TOO_HIGH                     (2133571348L)
#define EXT2_ET_INODE_BITMAP_WRITE               (2133571355L)
#define EXT2_ET_INODE_BITMAP_READ                (2133571356L)
#define EXT2_ET_BLOCK_BITMAP_WRITE               (2133571357L)
#define EXT2_ET_BLOCK_BITMAP_READ                (2133571358L)
#define EXT2_ET_INODE_TABLE_WRITE                (2133571359L)
#define EXT2_ET_INODE_TABLE_READ                 (2133571360L)
#define EXT2_ET_NEXT_INODE_READ                  (2133571361L)
#define EXT2_ET_UNEXPECTED_BLOCK_SIZE            (2133571362L)
#define EXT2_ET_DIR_CORRUPTED                    (2133571363L)
#define EXT2_ET_SHORT_READ                       (2133571364L)
#define EXT2_ET_SHORT_WRITE                      (2133571365L)
#define EXT2_ET_DIR_NO_SPACE                     (2133571366L)
#define EXT2_ET_NO_INODE_BITMAP                  (2133571367L)
#define EXT2_ET_NO_BLOCK_BITMAP                  (2133571368L)
#define EXT2_ET_BAD_INODE_MARK                   (2133571376L)
#define EXT2_ET_BAD_BLOCK_MARK                   (2133571373L)
#define EXT2_ET_BAD_BLOCK_UNMARK                 (2133571374L)
#define EXT2_ET_BAD_INODE_NUM                    (2133571369L)
#define EXT2_ET_BAD_BLOCK_NUM                    (2133571370L)
#define EXT2_ET_EXPAND_DIR_ERR                   (2133571371L)
#define EXT2_ET_TOOSMALL                         (2133571372L)
#define EXT2_ET_NEQ_BLOCK_BITMAP                 (2133571384L)
#define EXT2_ET_NEQ_INODE_BITMAP                 (2133571385L)
#define EXT2_ET_BAD_DEVICE_NAME                  (2133571386L)
#define EXT2_ET_MISSING_INODE_TABLE              (2133571387L)
#define EXT2_ET_CORRUPT_SUPERBLOCK               (2133571388L)
#define EXT2_ET_BAD_GENERIC_MARK                 (2133571389L)
#define EXT2_ET_BAD_GENERIC_UNMARK               (2133571390L)
#define EXT2_ET_BAD_GENERIC_TEST                 (2133571391L)
#define EXT2_ET_SYMLINK_LOOP                     (2133571392L)
#define EXT2_ET_CALLBACK_NOTHANDLED              (2133571393L)
#define EXT2_ET_BAD_BLOCK_IN_INODE_TABLE         (2133571394L)
#define EXT2_ET_UNSUPP_FEATURE                   (2133571395L)
#define EXT2_ET_RO_UNSUPP_FEATURE                (2133571396L)
#define EXT2_ET_LLSEEK_FAILED                    (2133571397L)
#define EXT2_ET_NO_MEMORY                        (2133571398L)
#define EXT2_ET_INVALID_ARGUMENT                 (2133571399L)
#define EXT2_ET_BLOCK_ALLOC_FAIL                 (2133571400L)
#define EXT2_ET_INODE_ALLOC_FAIL                 (2133571401L)
#define EXT2_ET_NO_DIRECTORY                     (2133571402L)
#define EXT2_ET_TOO_MANY_REFS                    (2133571403L)
//#define EXT2_ET_FILE_NOT_FOUND                   (2133571404L)
//#define EXT2_ET_FILE_RO                          (2133571405L)
#define EXT2_ET_DB_NOT_FOUND                     (2133571406L)
//#define EXT2_ET_DIR_EXISTS                       (2133571407L)
#define EXT2_ET_UNIMPLEMENTED                    (2133571408L)
#define EXT2_ET_CANCEL_REQUESTED                 (2133571409L)
#define EXT2_ET_FILE_TOO_BIG                     (2133571410L)
#define EXT2_ET_JOURNAL_NOT_BLOCK                (2133571411L)
#define EXT2_ET_NO_JOURNAL_SB                    (2133571412L)
#define EXT2_ET_JOURNAL_TOO_SMALL                (2133571413L)
#define EXT2_ET_JOURNAL_UNSUPP_VERSION           (2133571414L)
#define EXT2_ET_LOAD_EXT_JOURNAL                 (2133571415L)
#define EXT2_ET_NO_JOURNAL                       (2133571416L)
#define EXT2_ET_DIRHASH_UNSUPP                   (2133571417L)
#define EXT2_ET_BAD_EA_BLOCK_NUM                 (2133571418L)
#define EXT2_ET_TOO_MANY_INODES                  (2133571419L)
#define EXT2_ET_NOT_IMAGE_FILE                   (2133571420L)
#define EXT2_ET_RES_GDT_BLOCKS                   (2133571421L)
#define EXT2_ET_RESIZE_INODE_CORRUPT             (2133571422L)
#define EXT2_ET_SET_BMAP_NO_IND                  (2133571423L)

#define EXT2_ET_DBLIST_EMPTY                     (2133571434L)
#define EXT2_ET_RO_BLOCK_ITERATE                 (2133571435L)
#define EXT2_ET_MAGIC_EXTENT_PATH                (2133571436L)
#define EXT2_ET_MAGIC_GENERIC_BITMAP64           (2133571437L)
#define EXT2_ET_MAGIC_BLOCK_BITMAP64             (2133571438L)
#define EXT2_ET_MAGIC_INODE_BITMAP64             (2133571439L)

#define EXT2_ET_EXTENT_HEADER_BAD                (2133571447L)
#define EXT2_ET_EXTENT_INDEX_BAD                 (2133571448L)
#define EXT2_ET_EXTENT_LEAF_BAD                  (2133571449L)
#define EXT2_ET_EXTENT_NO_SPACE                  (2133571450L)
#define EXT2_ET_INODE_NOT_EXTENT                 (2133571451L)
#define EXT2_ET_OP_NOT_SUPPORTED                 (2133571457L)
#define EXT2_ET_CANT_INSERT_EXTENT               (2133571458L)
#define EXT2_ET_CANT_SPLIT_EXTENT                (2133571459L)
#define EXT2_ET_IO_CHANNEL_NO_SUPPORT_64         (2133571463L)
#define EXT2_ET_CANT_USE_LEGACY_BITMAPS          (2133571465L)

#define EXT2_ET_BAD_DESC_SIZE                    (2133571473L)

#define EXT2_ET_INODE_CSUM_INVALID               (2133571474L)
#define EXT2_ET_INODE_BITMAP_CSUM_INVALID        (2133571475L)
#define EXT2_ET_EXTENT_CSUM_INVALID              (2133571476L)
#define EXT2_ET_DIR_NO_SPACE_FOR_CSUM            (2133571477L)
#define EXT2_ET_DIR_CSUM_INVALID                 (2133571478L)
#define EXT2_ET_EXT_ATTR_CSUM_INVALID            (2133571479L)
#define EXT2_ET_SB_CSUM_INVALID                  (2133571480L)
#define EXT2_ET_UNKNOWN_CSUM                     (2133571481L)
#define EXT2_ET_MMP_CSUM_INVALID                 (2133571482L)
#define EXT2_ET_FILE_EXISTS                      (2133571483L)
#define EXT2_ET_BLOCK_BITMAP_CSUM_INVALID        (2133571484L)

#define EXT2_ET_INLINE_DATA_CANT_ITERATE         (2133571485L)
#define EXT2_ET_INODE_IS_GARBAGE                 (2133571497L)



#define EXT2_SEEK_SET	0
#define EXT2_SEEK_CUR	1
#define EXT2_SEEK_END	2

/*
 * Flags for the ext2_filsys structure and for ext2fs_open()
 */
#define EXT2_FLAG_RW			0x01
#define EXT2_FLAG_CHANGED		0x02
#define EXT2_FLAG_DIRTY			0x04
#define EXT2_FLAG_VALID			0x08
#define EXT2_FLAG_IB_DIRTY		0x10
#define EXT2_FLAG_BB_DIRTY		0x20
#define EXT2_FLAG_SWAP_BYTES		0x40
#define EXT2_FLAG_SWAP_BYTES_READ	0x80
#define EXT2_FLAG_SWAP_BYTES_WRITE	0x100
#define EXT2_FLAG_MASTER_SB_ONLY	0x200
#define EXT2_FLAG_FORCE			0x400
#define EXT2_FLAG_SUPER_ONLY		0x800
#define EXT2_FLAG_JOURNAL_DEV_OK	0x1000
#define EXT2_FLAG_IMAGE_FILE		0x2000
#define EXT2_FLAG_EXCLUSIVE		0x4000
#define EXT2_FLAG_SOFTSUPP_FEATURES	0x8000
#define EXT2_FLAG_NOFREE_ON_ERROR	0x10000
#define EXT2_FLAG_64BITS		0x20000
#define EXT2_FLAG_PRINT_PROGRESS	0x40000
#define EXT2_FLAG_DIRECT_IO		0x80000
#define EXT2_FLAG_IGNORE_CSUM_ERRORS	0x200000


/*
 * Return flags for the block iterator functions
 */
#define BLOCK_CHANGED                   1
#define BLOCK_ABORT                     2
#define BLOCK_ERROR                     4
#define BLOCK_INLINE_DATA_CHANGED       8

/*
 * Special inode numbers
 */
#define EXT2_BAD_INO		 1	/* Bad blocks inode */
#define EXT2_ROOT_INO		 2	/* Root inode */

/* First non-reserved inode for old ext2 filesystems */
#define EXT2_GOOD_OLD_FIRST_INO	11

/*
 * The second extended file system magic number
 */
#define EXT2_SUPER_MAGIC	0xEF53

/*
 * Macro-instructions used to manage several block sizes
 */
#define EXT2_MIN_BLOCK_LOG_SIZE		10	/* 1024 */
#define EXT2_MAX_BLOCK_LOG_SIZE		16	/* 65536 */
#define EXT2_MIN_BLOCK_SIZE	(1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE	(1 << EXT2_MAX_BLOCK_LOG_SIZE)
#define EXT2_BLOCK_SIZE(s)	(EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#define EXT2_BLOCK_SIZE_BITS(s)	((s)->s_log_block_size + 10)
#define EXT2_INODE_SIZE(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_SIZE : (s)->s_inode_size)
#define EXT2_FIRST_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_FIRST_INO : (s)->s_first_ino)
#define EXT2_ADDR_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / sizeof(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#define EXT2_CLUSTER_SIZE(s)		(EXT2_MIN_BLOCK_SIZE << \
						(s)->s_log_cluster_size)

#if defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
typedef struct stat64 ext2fs_struct_stat;
#else
typedef struct stat ext2fs_struct_stat;
#endif

#ifdef WORDS_BIGENDIAN
#define ext2fs_cpu_to_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#define ext2fs_le64_to_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#define ext2fs_cpu_to_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#define ext2fs_le32_to_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#define ext2fs_cpu_to_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#define ext2fs_le16_to_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#define ext2fs_cpu_to_be64(x) ((__force __be64)(__u64)(x))
#define ext2fs_be64_to_cpu(x) ((__force __u64)(__be64)(x))
#define ext2fs_cpu_to_be32(x) ((__force __be32)(__u32)(x))
#define ext2fs_be32_to_cpu(x) ((__force __u32)(__be32)(x))
#define ext2fs_cpu_to_be16(x) ((__force __be16)(__u16)(x))
#define ext2fs_be16_to_cpu(x) ((__force __u16)(__be16)(x))
#else
#define ext2fs_cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define ext2fs_le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define ext2fs_cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define ext2fs_le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define ext2fs_cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define ext2fs_le16_to_cpu(x) ((__force __u16)(__le16)(x))

#define ext2fs_cpu_to_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#define ext2fs_be64_to_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#define ext2fs_cpu_to_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#define ext2fs_be32_to_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#define ext2fs_cpu_to_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#define ext2fs_be16_to_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#endif


/*
 * Structure of a blocks group descriptor
 */
struct ext2_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_unused;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
struct ext4_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_unused;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_block_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_table_hi;	/* Inodes table block MSB */
	__u16	bg_free_blocks_count_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_count_hi;/* Free inodes count MSB */
	__u16	bg_used_dirs_count_hi;	/* Directories count MSB */
	__u16	bg_itable_unused_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

/*
 * Inode cache structure
 */
struct ext2_inode_cache_ent {
	ext2_ino_t		ino;
	struct ext2_inode	*inode;
};

struct ext2_inode_cache {
	void *				buffer;
	blk64_t				buffer_blk;
	int				cache_last;
	unsigned int			cache_size;
	int				refcount;
	struct ext2_inode_cache_ent	*cache;
};

#define EXT4_BG_INODE_BITMAP_CSUM_HI_END	\
	(offsetof(struct ext4_group_desc, bg_inode_bitmap_csum_hi) + \
	 sizeof(__u16))
#define EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(struct ext4_group_desc, bg_block_bitmap_csum_hi) + \
	 sizeof(__u16))

#define EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#define EXT2_BG_BLOCK_UNINIT	0x0002 /* Block bitmap not initialized */
#define EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

#define EXT2_SB(sb)     (sb)

/*
 * Macro-instructions used to manage group descriptors
 */
#define EXT2_MIN_DESC_SIZE             32
#define EXT2_MIN_DESC_SIZE_64BIT       64
#define EXT2_MAX_DESC_SIZE             EXT2_MIN_BLOCK_SIZE
#define EXT2_DESC_SIZE(s)                                                \
       ((EXT2_SB(s)->s_feature_incompat & EXT4_FEATURE_INCOMPAT_64BIT) ? \
	(s)->s_desc_size : EXT2_MIN_DESC_SIZE)

#define EXT2_BLOCKS_PER_GROUP(s)	(EXT2_SB(s)->s_blocks_per_group)
#define EXT2_INODES_PER_GROUP(s)	(EXT2_SB(s)->s_inodes_per_group)
#define EXT2_CLUSTERS_PER_GROUP(s)	(EXT2_SB(s)->s_clusters_per_group)
#define EXT2_INODES_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s)/EXT2_INODE_SIZE(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#define EXT2_MAX_BLOCKS_PER_GROUP(s)	((((unsigned) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_SIZE(s) / \
					  EXT2_BLOCK_SIZE(s)))
#define EXT2_MAX_CLUSTERS_PER_GROUP(s)	(((unsigned) 1 << 16) - 8)
#define EXT2_MAX_INODES_PER_GROUP(s)	(((unsigned) 1 << 16) - \
					 EXT2_INODES_PER_BLOCK(s))

#define EXT2_DESC_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / EXT2_DESC_SIZE(s))

#define EXT2_GROUPS_TO_BLOCKS(s, g)   ((blk64_t) EXT2_BLOCKS_PER_GROUP(s) * \
				       (g))
#define EXT2_GROUPS_TO_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_PER_GROUP(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#define EXT2_NDIR_BLOCKS		12
#define EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

/*
 * 64-bit bitmap backend types
 */
#define EXT2FS_BMAP64_BITARRAY	1
#define EXT2FS_BMAP64_RBTREE	2
#define EXT2FS_BMAP64_AUTODIR	3

/*
 * Return flags for the block iterator functions
 */
#define BLOCK_CHANGED			1
#define BLOCK_ABORT			2
#define BLOCK_ERROR			4
#define BLOCK_INLINE_DATA_CHANGED	8


#define SUPERBLOCK_OFFSET	1024
#define SUPERBLOCK_SIZE		1024

#define BLOCK_FLAG_APPEND	1
#define BLOCK_FLAG_HOLE		1
#define BLOCK_FLAG_DEPTH_TRAVERSE	2
#define BLOCK_FLAG_DATA_ONLY	4
#define BLOCK_FLAG_READ_ONLY	8

#define BLOCK_FLAG_NO_LARGE	0x1000

/*
 * Magic "block count" return values for the block iterator function.
 */
#define BLOCK_COUNT_IND		(-1)
#define BLOCK_COUNT_DIND	(-2)
#define BLOCK_COUNT_TIND	(-3)
#define BLOCK_COUNT_TRANSLATOR	(-4)

/*
 * Generic (non-filesystem layout specific) extents structure
 */

struct ext2fs_extent {
	blk64_t	e_pblk;		/* first physical block */
	blk64_t	e_lblk;		/* first logical block extent covers */
	__u32	e_len;		/* number of blocks covered by extent */
	__u32	e_flags;	/* extent flags */
};


/*
 * Flags for directory block reading and writing functions
 */
#define EXT2_DIRBLOCK_V2_STRUCT	0x0001

/*
 * Return flags for the directory iterator functions
 */
#define DIRENT_CHANGED	1
#define DIRENT_ABORT	2
#define DIRENT_ERROR	3

/*
 * Directory iterator flags
 */

#define DIRENT_FLAG_INCLUDE_EMPTY	1
#define DIRENT_FLAG_INCLUDE_REMOVED	2
#define DIRENT_FLAG_INCLUDE_CSUM	4
#define DIRENT_FLAG_INCLUDE_INLINE_DATA 8

#define DIRENT_DOT_FILE		1
#define DIRENT_DOT_DOT_FILE	2
#define DIRENT_OTHER_FILE	3
#define DIRENT_DELETED_FILE	4
#define DIRENT_CHECKSUM		5

/*
 * ext2fs_scan flags
 */
#define EXT2_SF_CHK_BADBLOCKS	0x0001
#define EXT2_SF_BAD_INODE_BLK	0x0002
#define EXT2_SF_BAD_EXTRA_BYTES	0x0004
#define EXT2_SF_SKIP_MISSING_ITABLE	0x0008
#define EXT2_SF_DO_LAZY		0x0010
#define EXT2_SF_WARN_GARBAGE_INODES	0x0020

/*
 * ext2fs_check_if_mounted flags
 */
#define EXT2_MF_MOUNTED		1
#define EXT2_MF_ISROOT		2
#define EXT2_MF_READONLY	4
#define EXT2_MF_SWAP		8
#define EXT2_MF_BUSY		16

/*
 * Ext2/linux mode flags.  We define them here so that we don't need
 * to depend on the OS's sys/stat.h, since we may be compiling on a
 * non-Linux system.
 */
#define LINUX_S_IFMT  00170000
#define LINUX_S_IFSOCK 0140000
#define LINUX_S_IFLNK	 0120000
#define LINUX_S_IFREG  0100000
#define LINUX_S_IFBLK  0060000
#define LINUX_S_IFDIR  0040000
#define LINUX_S_IFCHR  0020000
#define LINUX_S_IFIFO  0010000
#define LINUX_S_ISUID  0004000
#define LINUX_S_ISGID  0002000
#define LINUX_S_ISVTX  0001000

#define LINUX_S_IRWXU 00700
#define LINUX_S_IRUSR 00400
#define LINUX_S_IWUSR 00200
#define LINUX_S_IXUSR 00100

#define LINUX_S_IRWXG 00070
#define LINUX_S_IRGRP 00040
#define LINUX_S_IWGRP 00020
#define LINUX_S_IXGRP 00010

#define LINUX_S_IRWXO 00007
#define LINUX_S_IROTH 00004
#define LINUX_S_IWOTH 00002
#define LINUX_S_IXOTH 00001

#define LINUX_S_ISLNK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#define LINUX_S_ISREG(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#define LINUX_S_ISDIR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#define LINUX_S_ISCHR(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#define LINUX_S_ISBLK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#define LINUX_S_ISFIFO(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#define LINUX_S_ISSOCK(m)	(((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

/*
 * ext2 size of an inode
 */
#define EXT2_I_SIZE(i)	((i)->i_size | ((__u64) (i)->i_size_high << 32))

typedef struct ext2_icount *ext2_icount_t;

/*
 * Flags for ext2fs_bmap
 */
#define BMAP_ALLOC	0x0001
#define BMAP_SET	0x0002
#define BMAP_UNINIT	0x0004
#define BMAP_ZERO	0x0008

/*
 * Returned flags from ext2fs_bmap
 */
#define BMAP_RET_UNINIT	0x0001

/*
 * Flags for imager.c functions
 */
#define IMAGER_FLAG_INODEMAP	1
#define IMAGER_FLAG_SPARSEWRITE	2

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  if ((struct)->magic != (code)) return (code)

#ifdef CONFIG_MMP
#define EXT4_LIB_INCOMPAT_MMP		EXT4_FEATURE_INCOMPAT_MMP
#else
#define EXT4_LIB_INCOMPAT_MMP		(0)
#endif

/*
 * These features are only allowed if EXT2_FLAG_SOFTSUPP_FEATURES is passed
 * to ext2fs_openfs()
 */
#define EXT2_LIB_SOFTSUPP_INCOMPAT	(0)
#define EXT2_LIB_SOFTSUPP_RO_COMPAT	(EXT4_FEATURE_RO_COMPAT_REPLICA)


/* Translate a block number to a cluster number */
#define EXT2FS_CLUSTER_RATIO(fs)	(1 << (fs)->cluster_ratio_bits)
#define EXT2FS_CLUSTER_MASK(fs)		(EXT2FS_CLUSTER_RATIO(fs) - 1)
#define EXT2FS_B2C(fs, blk)		((blk) >> (fs)->cluster_ratio_bits)

/*
 * Structure of an inode on the disk
 */
struct ext2_inode {
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union {
		struct {
			__u32	l_i_version; /* was l_i_reserved1 */
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_size_high;	/* Formerly i_dir_acl, directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union {
		struct {
			__u16	l_i_blocks_hi;
			__u16	l_i_file_acl_high;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		struct {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
struct ext2_inode_large {
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union {
		struct {
			__u32	l_i_version; /* was l_i_reserved1 */
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_size_high;	/* Formerly i_dir_acl, directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union {
		struct {
			__u16	l_i_blocks_hi;
			__u16	l_i_file_acl_high;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		struct {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
	__u16	i_extra_isize;
	__u16	i_checksum_hi;	/* crc32c(uuid+inum+inode) */
	__u32	i_ctime_extra;	/* extra Change time (nsec << 2 | epoch) */
	__u32	i_mtime_extra;	/* extra Modification time (nsec << 2 | epoch) */
	__u32	i_atime_extra;	/* extra Access time (nsec << 2 | epoch) */
	__u32	i_crtime;	/* File creation time */
	__u32	i_crtime_extra;	/* extra File creation time (nsec << 2 | epoch)*/
	__u32	i_version_hi;	/* high 32 bits for 64-bit version */
	__u32   i_projid;       /* Project ID */
};

#define EXT4_INODE_CSUM_HI_EXTRA_END	\
	(offsetof(struct ext2_inode_large, i_checksum_hi) + sizeof(__u16) - \
	 EXT2_GOOD_OLD_INODE_SIZE)

#define i_dir_acl	i_size_high

#define i_checksum_lo	osd2.linux2.l_i_checksum_lo

#define EXT4_KEY_DESCRIPTOR_SIZE		8
#define EXT4_CRYPTO_BLOCK_SIZE			16

/*
 * Structure of the super block
 */
struct ext2_super_block {
	__u32	s_inodes_count;		/* Inodes count */
	__u32	s_blocks_count;		/* Blocks count */
	__u32	s_r_blocks_count;	/* Reserved blocks count */
	__u32	s_free_blocks_count;	/* Free blocks count */
	__u32	s_free_inodes_count;	/* Free inodes count */
	__u32	s_first_data_block;	/* First Data Block */
	__u32	s_log_block_size;	/* Block size */
	__u32	s_log_cluster_size;	/* Allocation cluster size */
	__u32	s_blocks_per_group;	/* # Blocks per group */
	__u32	s_clusters_per_group;	/* # Fragments per group */
	__u32	s_inodes_per_group;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_count;		/* Mount count */
	__s16	s_max_mnt_count;	/* Maximal mount count */
	__u16	s_magic;		/* Magic signature */
	__u16	s_state;		/* File system state */
	__u16	s_errors;		/* Behaviour when detecting errors */
	__u16	s_minor_rev_level;	/* minor revision level */
	__u32	s_lastcheck;		/* time of last check */
	__u32	s_checkinterval;	/* max. time between checks */
	__u32	s_creator_os;		/* OS */
	__u32	s_rev_level;		/* Revision level */
	__u16	s_def_resuid;		/* Default uid for reserved blocks */
	__u16	s_def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT2_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 *
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	__u32	s_first_ino;		/* First non-reserved inode */
	__u16   s_inode_size;		/* size of inode structure */
	__u16	s_block_group_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
	__u8	s_uuid[16];		/* 128-bit uuid for volume */
	char	s_volume_name[16];	/* volume name */
	char	s_last_mounted[64];	/* directory where last mounted */
	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_blocks;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
	__u8	s_journal_uuid[16];	/* uuid of journal superblock */
	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_last_orphan;		/* start of list of inodes to delete */
	__u32	s_hash_seed[4];		/* HTREE hash seed */
	__u8	s_def_hash_version;	/* Default hash version to use */
	__u8	s_jnl_backup_type; 	/* Default type of journal backup */
	__u16	s_desc_size;		/* Group desc. size: INCOMPAT_64BIT */
	__u32	s_default_mount_opts;
	__u32	s_first_meta_bg;	/* First metablock group */
	__u32	s_mkfs_time;		/* When the filesystem was created */
	__u32	s_jnl_blocks[17]; 	/* Backup of the journal inode */
	__u32	s_blocks_count_hi;	/* Blocks count high 32bits */
	__u32	s_r_blocks_count_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_blocks_hi; 	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize; 	/* New inodes should reserve # bytes */
	__u32	s_flags;		/* Miscellaneous flags */
	__u16   s_raid_stride;		/* RAID stride */
	__u16   s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64   s_mmp_block;            /* Block for multi-mount protection */
	__u32   s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_per_flex;	/* FLEX_BG group size */
	__u8    s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_blocks_count; /* reserved blocks for active
					      snapshot's future use */
	__u32	s_snapshot_list;	/* inode number of the head of the on-disk snapshot list */
#define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
	__u32	s_error_count;		/* number of fs errors */
	__u32	s_first_error_time;	/* first time an error happened */
	__u32	s_first_error_ino;	/* inode involved in first error */
	__u64	s_first_error_block;	/* block involved of first error */
	__u8	s_first_error_func[32];	/* function where the error happened */
	__u32	s_first_error_line;	/* line number where error happened */
	__u32	s_last_error_time;	/* most recent time of an error */
	__u32	s_last_error_ino;	/* inode involved in last error */
	__u32	s_last_error_line;	/* line number where error happened */
	__u64	s_last_error_block;	/* block involved of last error */
	__u8	s_last_error_func[32];	/* function where the error happened */
#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)
	__u8	s_mount_opts[64];
	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_blocks;	/* overhead blocks/clusters in fs */
	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
	__le32	s_checksum_seed;	/* crc32c(orig_uuid) if csum_seed set */
	__le32	s_reserved[98];		/* Padding to the end of the block */
	__u32	s_checksum;		/* crc32c(superblock) */
};

#define EXT4_S_ERR_LEN (EXT4_S_ERR_END - EXT4_S_ERR_START)

/*
 * Structure of a directory entry
 */
#define EXT2_NAME_LEN 255

struct ext2_dir_entry {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	char	name[EXT2_NAME_LEN];	/* File name */
};

struct struct_io_stats {
        int                     num_fields;
        int                     reserved;
        unsigned long long      bytes_read;
        unsigned long long      bytes_written;
};
typedef struct struct_io_stats *io_stats;

typedef struct struct_io_manager *io_manager;
typedef struct struct_io_channel *io_channel;
struct struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	char		*name;
	int		block_size;
	errcode_t	(*read_error)(io_channel channel,
				      unsigned long block,
				      int count,
				      void *data,
				      size_t size,
				      int actual_bytes_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       unsigned long block,
				       int count,
				       const void *data,
				       size_t size,
				       int actual_bytes_written,
				       errcode_t error);
	int		refcount;
	int		flags;
	long		reserved[14];
	void		*private_data;
	void		*app_data;
	int		align;
};

struct struct_io_manager {
	errcode_t magic;
	const char *name;
	errcode_t (*open)(const char *name, int flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*set_blksize)(io_channel channel, int blksize);
	errcode_t (*read_blk)(io_channel channel, unsigned long block,
			      int count, void *data);
	errcode_t (*write_blk)(io_channel channel, unsigned long block,
			       int count, const void *data);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, unsigned long offset,
				int count, const void *data);
	errcode_t (*set_option)(io_channel channel, const char *option,
				const char *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, unsigned long long block,
					int count, void *data);
	errcode_t (*write_blk64)(io_channel channel, unsigned long long block,
					int count, const void *data);
	errcode_t (*discard)(io_channel channel, unsigned long long block,
			     unsigned long long count);
	errcode_t (*cache_readahead)(io_channel channel,
				     unsigned long long block,
				     unsigned long long count);
	errcode_t (*zeroout)(io_channel channel, unsigned long long block,
			     unsigned long long count);
	long	reserved[14];
};

typedef struct ext2_struct_u32_list *badblocks_list;
typedef struct ext2_struct_u32_list *ext2_badblocks_list;
typedef struct ext2fs_struct_generic_bitmap *ext2fs_generic_bitmap;
typedef struct ext2fs_struct_generic_bitmap *ext2fs_inode_bitmap;
typedef struct ext2fs_struct_generic_bitmap *ext2fs_block_bitmap;
typedef struct ext2_struct_dblist *ext2_dblist;
typedef struct struct_ext2_filsys *ext2_filsys;

struct struct_ext2_filsys {
	errcode_t			magic;
	io_channel			io;
	int				flags;
	char *				device_name;
	struct ext2_super_block	* 	super;
	unsigned int			blocksize;
	int				fragsize;
	dgrp_t				group_desc_count;
	unsigned long			desc_blocks;
	struct opaque_ext2_group_desc *	group_desc;
	unsigned int			inode_blocks_per_group;
	ext2fs_inode_bitmap		inode_map;
	ext2fs_block_bitmap		block_map;
	/* XXX FIXME-64: not 64-bit safe, but not used? */
	errcode_t (*get_blocks)(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks);
	errcode_t (*check_directory)(ext2_filsys fs, ext2_ino_t ino);
	errcode_t (*write_bitmaps)(ext2_filsys fs);
	errcode_t (*read_inode)(ext2_filsys fs, ext2_ino_t ino,
				struct ext2_inode *inode);
	errcode_t (*write_inode)(ext2_filsys fs, ext2_ino_t ino,
				struct ext2_inode *inode);
	ext2_badblocks_list		badblocks;
	ext2_dblist			dblist;
	__u32				stride;	/* for mke2fs */
	struct ext2_super_block *	orig_super;
	__u32				umask;
	time_t				now;
	int				cluster_ratio_bits;
	__u16				default_bitmap_type;
	__u16				pad;
	/*
	 * Reserved for future expansion
	 */
	__u32				reserved[5];

	/*
	 * Reserved for the use of the calling application.
	 */
	void *				priv_data;

	/*
	 * Inode cache
	 */
	struct ext2_inode_cache		*icache;
	io_channel			image_io;

	/*
	 * More callback functions
	 */
	errcode_t (*get_alloc_block)(ext2_filsys fs, blk64_t goal,
				     blk64_t *ret);
	void (*block_alloc_stats)(ext2_filsys fs, blk64_t blk, int inuse);

	/*
	 * Buffers for Multiple mount protection(MMP) block.
	 */
	void *mmp_buf;
	void *mmp_cmp;
	int mmp_fd;

	/*
	 * Time at which e2fsck last updated the MMP block.
	 */
	long mmp_last_written;

	/* Precomputed FS UUID checksum for seeding other checksums */
	__u32 csum_seed;

	io_channel			journal_io;
	char				*journal_name;

	/* New block range allocation hooks */
	errcode_t (*new_range)(ext2_filsys fs, int flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen);
	void (*block_alloc_stats_range)(ext2_filsys fs, blk64_t blk, blk_t num,
					int inuse);
};

struct ext2fs_struct_generic_bitmap {
	errcode_t		magic;
	ext2_filsys 		fs;
	struct ext2_bitmap_ops	*bitmap_ops;
	int			flags;
	__u64			start, end;
	__u64			real_end;
	int			cluster_bits;
	char			*description;
	void			*private;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	struct ext2_bmap_statistics	stats;
#endif
};

/*
 * Codes for operating systems
 */
#define EXT2_OS_LINUX		0
#define EXT2_OS_HURD		1
#define EXT2_OBSO_OS_MASIX	2
#define EXT2_OS_FREEBSD		3
#define EXT2_OS_LITES		4

/*
 * Revision levels
 */
#define EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#define EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#define EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#define EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#define EXT2_GOOD_OLD_INODE_SIZE 128

#define EXT2_LIB_CURRENT_REV	EXT2_DYNAMIC_REV

/*
 * Feature set definitions
 */

#define EXT2_HAS_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_compat & (mask) )
#define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (mask) )
#define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_incompat & (mask) )

#define EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020
#define EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#define EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#define EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200


#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#define EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#define EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#define EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#define EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#define EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#define EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#define EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#define EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#define EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#define EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */


#define EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#define EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#define EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#define EXT4_FEATURE_INCOMPAT_64BIT		0x0080
#define EXT4_FEATURE_INCOMPAT_MMP		0x0100
#define EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#define EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#define EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#define EXT4_FEATURE_INCOMPAT_CSUM_SEED		0x2000
#define EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#define EXT4_FEATURE_INCOMPAT_INLINE_DATA	0x8000 /* data in inode */
#define EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000


/*
 * Inode flags
 */
#define EXT2_COMPR_FL			0x00000004 /* Compress file */
#define EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#define EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#define EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#define EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#define EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT2_DIRTY_FL			0x00000100
#define EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#define EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#define EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#define EXT2_BTREE_FL			0x00001000 /* btree format dir */
#define EXT2_INDEX_FL			0x00001000 /* hash-indexed directory */
#define EXT2_IMAGIC_FL			0x00002000
#define EXT3_JOURNAL_DATA_FL		0x00004000 /* file data should be journaled */
#define EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#define EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#define EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#define EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#define EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#define EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#define EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#define EXT4_INLINE_DATA_FL		0x10000000 /* Inode has inline data */
#define EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */


#define EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
	return ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \

#define EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
	return ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \

#define EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
	return ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_index,		2, DIR_INDEX)
EXT4_FEATURE_COMPAT_FUNCS(lazy_bg,		2, LAZY_BG)
EXT4_FEATURE_COMPAT_FUNCS(exclude_bitmap,	2, EXCLUDE_BITMAP)
EXT4_FEATURE_COMPAT_FUNCS(sparse_super2,	4, SPARSE_SUPER2)

EXT4_FEATURE_RO_COMPAT_FUNCS(sparse_super,	2, SPARSE_SUPER)
EXT4_FEATURE_RO_COMPAT_FUNCS(large_file,	2, LARGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(huge_file,		4, HUGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(gdt_csum,		4, GDT_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(dir_nlink,		4, DIR_NLINK)
EXT4_FEATURE_RO_COMPAT_FUNCS(extra_isize,	4, EXTRA_ISIZE)
EXT4_FEATURE_RO_COMPAT_FUNCS(has_snapshot,	4, HAS_SNAPSHOT)
EXT4_FEATURE_RO_COMPAT_FUNCS(quota,		4, QUOTA)
EXT4_FEATURE_RO_COMPAT_FUNCS(bigalloc,		4, BIGALLOC)
EXT4_FEATURE_RO_COMPAT_FUNCS(metadata_csum,	4, METADATA_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(replica,		4, REPLICA)
EXT4_FEATURE_RO_COMPAT_FUNCS(readonly,		4, READONLY)
EXT4_FEATURE_RO_COMPAT_FUNCS(project,		4, PROJECT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64bit,		4, 64BIT)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_seed,		4, CSUM_SEED)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_data,	4, INLINE_DATA)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)

void reset_getopt(void);
unsigned long parse_ulong(const char *str, const char *cmd,
				 const char *descr, int *err);
unsigned long long parse_ulonglong(const char *str, const char *cmd,
					  const char *descr, int *err);
int strtoblk(const char *cmd, const char *str, const char *errmsg,
		    blk64_t *ret);
int ext2_file_type(unsigned int mode);

void do_icheck(int argc, char **argv);

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
unsigned long parse_ulong(const char *str, const char *cmd,
			  const char *descr, int *err)
{
	char		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	if (*tmp == 0) {
		if (err)
			*err = 0;
		return ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	if (err)
		*err = 1;
	else
		exit(1);
	return 0;
}

/*
 * This function will convert a string to an unsigned long long, printing
 * an error message if it fails, and returning success or failure in err.
 */
unsigned long long parse_ulonglong(const char *str, const char *cmd,
				   const char *descr, int *err)
{
	char			*tmp;
	unsigned long long	ret;

	ret = strtoull(str, &tmp, 0);
	if (*tmp == 0) {
		if (err)
			*err = 0;
		return ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	if (err)
		*err = 1;
	else
		exit(1);
	return 0;
}


/*
 * This function will convert a string to a block number.  It returns
 * 0 on success, 1 on failure.  On failure, it outputs either an optionally
 * specified error message or a default.
 */
int strtoblk(const char *cmd, const char *str, const char *errmsg,
	     blk64_t *ret)
{
	blk64_t	blk;
	int	err;

	if (errmsg == NULL)
		blk = parse_ulonglong(str, cmd, "block number", &err);
	else
		blk = parse_ulonglong(str, cmd, errmsg, &err);
	*ret = blk;
	return err;
}


#define CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#define CHANNEL_FLAGS_BLOCK_DEVICE	0x04

extern errcode_t ext2fs_get_mem(unsigned long size, void *ptr);
extern errcode_t ext2fs_get_memzero(unsigned long size, void *ptr);
extern errcode_t ext2fs_get_array(unsigned long count,
				  unsigned long size, void *ptr);
extern errcode_t ext2fs_get_arrayzero(unsigned long count,
				      unsigned long size, void *ptr);
extern errcode_t ext2fs_free_mem(void *ptr);
extern errcode_t ext2fs_resize_mem(unsigned long old_size,
				   unsigned long size, void *ptr);
errcode_t ext2fs_get_memalign(unsigned long size,
                              unsigned long align, void *ptr);

blk64_t ext2fs_blocks_count(struct ext2_super_block *super);
dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t blk);
blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group);
blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group);
int ext2fs_group_blocks_count(ext2_filsys fs, dgrp_t group);
blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					struct ext2_inode *inode);
__u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group);

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t blk)
{
	return (blk - fs->super->s_first_data_block) /
		fs->super->s_blocks_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group)
{
	return fs->super->s_first_data_block +
		EXT2_GROUPS_TO_BLOCKS(fs->super, group);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group)
{
	return (group == fs->group_desc_count - 1 ?
		ext2fs_blocks_count(fs->super) - 1 :
		ext2fs_group_first_block2(fs, group) +
			(fs->super->s_blocks_per_group - 1));
}

/*
 * Return the number of blocks in a group
 */
int ext2fs_group_blocks_count(ext2_filsys fs, dgrp_t group)
{
	int num_blocks;

	if (group == fs->group_desc_count - 1) {
		num_blocks = (ext2fs_blocks_count(fs->super) -
				fs->super->s_first_data_block) %
			      fs->super->s_blocks_per_group;
		if (!num_blocks)
			num_blocks = fs->super->s_blocks_per_group;
	} else
		num_blocks = fs->super->s_blocks_per_group;

	return num_blocks;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					struct ext2_inode *inode)
{
	return (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_blocks_hi << 32 : 0)) -
		(inode->i_file_acl ? fs->blocksize >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
					struct ext2_inode *inode)
{
	return (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_blocks_hi << 32 : 0));
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_blocks_count(struct ext2_super_block *super)
{
	return super->s_blocks_count |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_blocks_count_hi << 32 : 0);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_blocks_count(struct ext2_super_block *super)
{
	return super->s_r_blocks_count |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_blocks_count_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
void ext2fs_r_blocks_count_set(struct ext2_super_block *super, blk64_t blk)
{
	super->s_r_blocks_count = blk;
	if (ext2fs_has_feature_64bit(super))
		super->s_r_blocks_count_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
void ext2fs_r_blocks_count_add(struct ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_blocks_count(super) + blk;
	ext2fs_r_blocks_count_set(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_blocks_count(struct ext2_super_block *super)
{
	return super->s_free_blocks_count |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_blocks_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
void ext2fs_free_blocks_count_set(struct ext2_super_block *super, blk64_t blk)
{
	super->s_free_blocks_count = blk;
	if (ext2fs_has_feature_64bit(super))
		super->s_free_blocks_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
void ext2fs_free_blocks_count_add(struct ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_blocks_count(super) + blk;
	ext2fs_free_blocks_count_set(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
struct ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  struct opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	return (struct ext2_group_desc *)((char *)gdp +
					  group * EXT2_DESC_SIZE(fs->super));
}

/* Do the same but as an ext4 group desc for internal use here */
static struct ext4_group_desc *ext4fs_group_desc(ext2_filsys fs,
					  struct opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	return (struct ext4_group_desc *)ext2fs_group_desc(fs, gdp, group);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;
	__u32 csum;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	csum = gdp->bg_block_bitmap_csum_lo;
	if (fs->super->s_desc_size >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_block_bitmap_csum_hi << 16);
	return csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_block_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_block_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
void ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_block_bitmap = blk;
	if (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_block_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

int ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    char *bitmap, int size)
{
	struct ext4_group_desc *gdp = (struct ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	if (!ext2fs_has_feature_metadata_csum(fs->super))
		return 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned char *)bitmap,
				      size);
	if (fs->super->s_desc_size >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	else
		calculated &= 0xFFFF;

	return provided == calculated;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_inode_table |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_table_hi << 32 : 0);
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_count(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_free_inodes_count |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_count_hi << 16 : 0);
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_itable_unused |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_unused_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
void ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_itable_unused = n;
	if (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_unused_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
void ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags = 0;
	return;
}

/*
 * Get the value of a particular flag for this block group
 */
int ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
void ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags |= bg_flags;
	return;
}

/*
 * Clear a flag or set of flags for this block group
 */
void ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags &= ~bg_flags;
	return;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	return gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
void ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum)
{
	struct ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_checksum = checksum;
	return;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_block(ext2_filsys fs, const struct ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	if (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_high) << 32;
	return blk;
}

/*
 * Set the acl block of a file
 */
void ext2fs_file_acl_block_set(ext2_filsys fs, struct ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	if (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_high = (__u64) blk >> 32;
}

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

struct ext2_bitmap_ops {
	int	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap bmap);
	void	(*free_bmap)(ext2fs_generic_bitmap bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	int	(*mark_bmap)(ext2fs_generic_bitmap bitmap, __u64 arg);
	int	(*unmark_bmap)(ext2fs_generic_bitmap bitmap, __u64 arg);
	int	(*test_bmap)(ext2fs_generic_bitmap bitmap, __u64 arg);
	void	(*mark_bmap_extent)(ext2fs_generic_bitmap bitmap, __u64 arg,
				    unsigned int num);
	void	(*unmark_bmap_extent)(ext2fs_generic_bitmap bitmap, __u64 arg,
				      unsigned int num);
	int	(*test_clear_bmap_extent)(ext2fs_generic_bitmap bitmap,
					  __u64 arg, unsigned int num);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap bitmap,
				    __u64 start, size_t num, void *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap bitmap,
				    __u64 start, size_t num, void *out);
	void (*clear_bmap)(ext2fs_generic_bitmap bitmap);
	void (*print_stats)(ext2fs_generic_bitmap);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

void ext2fs_free_generic_bmap(ext2fs_generic_bitmap bmap)
{
	if (!bmap)
		return;

	if (!EXT2FS_IS_64_BITMAP(bmap))
		return;

#ifdef ENABLE_BMAP_STATS
	if (getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#endif

	//bmap->bitmap_ops->free_bmap(bmap);

	if (bmap->description) {
		ext2fs_free_mem(&bmap->description);
		bmap->description = 0;
	}
	bmap->magic = 0;
	ext2fs_free_mem(&bmap);
}

void ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

void ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}


static errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	if (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		return EXT2_ET_MAGIC_GENERIC_BITMAP;
	return 0;
}

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

struct ext2fs_ba_private_struct {
	char *bitarray;
};

typedef struct ext2fs_ba_private_struct *ext2fs_ba_private;

static errcode_t ba_alloc_private_data (ext2fs_generic_bitmap bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(sizeof (ext2fs_ba_private), &bp);
	if (retval)
		return retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	if (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		return retval;
	}
	bitmap->private = (void *) bp;
	return 0;
}

static errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	if (retval)
		return retval;

	bp = (ext2fs_ba_private) bitmap->private;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	return 0;
}

static void ba_free_bmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	if (!bp)
		return;

	if (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

static errcode_t ba_copy_bmap(ext2fs_generic_bitmap src,
			      ext2fs_generic_bitmap dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->private;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	if (retval)
		return retval;

	dest_bp = (ext2fs_ba_private) dest->private;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	return 0;
}

static errcode_t ba_resize_bmap(ext2fs_generic_bitmap bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->private;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	if (new_end > bmap->end) {
		bitno = bmap->real_end;
		if (bitno > new_end)
			bitno = new_end;
		for (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	if (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		return 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	if (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		if (retval)
			return retval;
	}
	if (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	return 0;

}

/*
 * C-only 64 bit ops.
 */

int ext2fs_set_bit64(__u64 nr, void * addr)
{
	int		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	return retval;
}

int ext2fs_clear_bit64(__u64 nr, void * addr)
{
	int		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	return retval;
}

int ext2fs_test_bit64(__u64 nr, const void * addr)
{
	int			mask;
	const unsigned char	*ADDR = (const unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	return (mask & *ADDR);
}

_INLINE_ void ext2fs_fast_set_bit64(__u64 nr, void * addr)
{
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned char) (1 << (nr & 0x07));
}

_INLINE_ void ext2fs_fast_clear_bit64(__u64 nr, void * addr)
{
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned char) ~(1 << (nr & 0x07));
}

_INLINE_ int ext2fs_dirent_file_type(const struct ext2_dir_entry *entry)
{
	return entry->name_len >> 8;
}

_INLINE_ int ext2fs_dirent_name_len(const struct ext2_dir_entry *entry)
{
	return entry->name_len & 0xff;
}

static int ba_mark_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	return ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

static int ba_unmark_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	return ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

static int ba_test_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	return ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

static void ba_mark_bmap_extent(ext2fs_generic_bitmap bitmap, __u64 arg,
				unsigned int num)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;
	unsigned int i;

	for (i = 0; i < num; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

static void ba_unmark_bmap_extent(ext2fs_generic_bitmap bitmap, __u64 arg,
				  unsigned int num)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;
	unsigned int i;

	for (i = 0; i < num; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

static int ba_test_clear_bmap_extent(ext2fs_generic_bitmap bitmap,
				     __u64 start, unsigned int len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	__u64 start_byte, len_byte = len >> 3;
	unsigned int start_bit, len_bit = len % 8;
	unsigned int first_bit = 0;
	unsigned int last_bit  = 0;
	int mark_count = 0;
	int mark_bit = 0;
	int i;
	const char *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	if (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		if (len < 8 - start_bit) {
			mark_count = (int)len;
			mark_bit = len + start_bit - 1;
		} else
			mark_bit = 7;

		for (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		if (first_bit & ADDR[start_byte])
			return 0;
		else if (len <= 8 - start_bit)
			return 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	if (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		for (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		if (last_bit & ADDR[start_byte + len_byte])
			return 0;
		else if (len_byte == 0)
			return 1;
	}

	/* Check whether all bytes are 0 */
	return ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


static errcode_t ba_set_bmap_range(ext2fs_generic_bitmap bitmap,
				     __u64 start, size_t num, void *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memcpy (bp->bitarray + (start >> 3), in, (num + 7) >> 3);

	return 0;
}

static errcode_t ba_get_bmap_range(ext2fs_generic_bitmap bitmap,
				     __u64 start, size_t num, void *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memcpy (out, bp->bitarray + (start >> 3), (num + 7) >> 3);

	return 0;
}

static void ba_clear_bmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
static void ba_print_stats(ext2fs_generic_bitmap bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n",
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		sizeof(struct ext2fs_ba_private_struct));
}
#else
static void ba_print_stats(ext2fs_generic_bitmap bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
static errcode_t ba_find_first_zero(ext2fs_generic_bitmap bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->private;
	unsigned long bitpos = start - bitmap->start;
	unsigned long count = end - start + 1;
	int byte_found = 0; /* whether a != 0xff byte has been found */
	const unsigned char *pos;
	unsigned long max_loop_count, i;

	/* scan bits until we hit a byte boundary */
	while ((bitpos & 0x7) != 0 && count > 0) {
		if (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			return 0;
		}
		bitpos++;
		count--;
	}

	if (!count)
		return ENOENT;

	pos = ((unsigned char *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	while (count >= 8 && (((unsigned long)pos) & 0x07)) {
		if (*pos != 0xff) {
			byte_found = 1;
			break;
		}
		pos++;
		count -= 8;
		bitpos += 8;
	}

	if (!byte_found) {
		max_loop_count = count >> 6; /* 8-byte blocks */
		i = max_loop_count;
		while (i) {
			if (*((const __u64 *)pos) != ((__u64)-1))
				break;
			pos += 8;
			i--;
		}
		count -= 64 * (max_loop_count - i);
		bitpos += 64 * (max_loop_count - i);

		max_loop_count = count >> 3;
		i = max_loop_count;
		while (i) {
			if (*pos != 0xff) {
				byte_found = 1;
				break;
			}
			pos++;
			i--;
		}
		count -= 8 * (max_loop_count - i);
		bitpos += 8 * (max_loop_count - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	while (count-- > 0) {
		if (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			return 0;
		}
		bitpos++;
	}

	return ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
static errcode_t ba_find_first_set(ext2fs_generic_bitmap bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->private;
	unsigned long bitpos = start - bitmap->start;
	unsigned long count = end - start + 1;
	int byte_found = 0; /* whether a != 0xff byte has been found */
	const unsigned char *pos;
	unsigned long max_loop_count, i;

	/* scan bits until we hit a byte boundary */
	while ((bitpos & 0x7) != 0 && count > 0) {
		if (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			return 0;
		}
		bitpos++;
		count--;
	}

	if (!count)
		return ENOENT;

	pos = ((unsigned char *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	while (count >= 8 && (((unsigned long)pos) & 0x07)) {
		if (*pos != 0) {
			byte_found = 1;
			break;
		}
		pos++;
		count -= 8;
		bitpos += 8;
	}

	if (!byte_found) {
		max_loop_count = count >> 6; /* 8-byte blocks */
		i = max_loop_count;
		while (i) {
			if (*((const __u64 *)pos) != 0)
				break;
			pos += 8;
			i--;
		}
		count -= 64 * (max_loop_count - i);
		bitpos += 64 * (max_loop_count - i);

		max_loop_count = count >> 3;
		i = max_loop_count;
		while (i) {
			if (*pos != 0) {
				byte_found = 1;
				break;
			}
			pos++;
			i--;
		}
		count -= 8 * (max_loop_count - i);
		bitpos += 8 * (max_loop_count - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	while (count-- > 0) {
		if (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			return 0;
		}
		bitpos++;
	}

	return ENOENT;
}

struct ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
	.type = EXT2FS_BMAP64_BITARRAY,
	.new_bmap = ba_new_bmap,
	.free_bmap = ba_free_bmap,
	.copy_bmap = ba_copy_bmap,
	.resize_bmap = ba_resize_bmap,
	.mark_bmap = ba_mark_bmap,
	.unmark_bmap = ba_unmark_bmap,
	.test_bmap = ba_test_bmap,
	.test_clear_bmap_extent = ba_test_clear_bmap_extent,
	.mark_bmap_extent = ba_mark_bmap_extent,
	.unmark_bmap_extent = ba_unmark_bmap_extent,
	.set_bmap_range = ba_set_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.find_first_zero = ba_find_first_zero,
	.find_first_set = ba_find_first_set
};
#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({			\
	const __typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct rb_node
{
	unsigned long  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

struct rb_root
{
	struct rb_node *rb_node;
};


#define ext2fs_rb_parent(r)   ((struct rb_node *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  do { (r)->rb_parent_color &= ~1; } while (0)
#define ext2fs_rb_set_black(r)  do { (r)->rb_parent_color |= 1; } while (0)

static inline void ext2fs_rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long)p;
}
static inline void ext2fs_rb_set_color(struct rb_node *rb, int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ROOT	(struct rb_root) { NULL, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

static inline int ext2fs_rb_empty_root(struct rb_root *root)
{
	return root->rb_node == NULL;
}

static inline int ext2fs_rb_empty_node(struct rb_node *node)
{
	return ext2fs_rb_parent(node) == node;
}

static inline void ext2fs_rb_clear_node(struct rb_node *node)
{
	ext2fs_rb_set_parent(node, node);
}

extern void ext2fs_rb_insert_color(struct rb_node *, struct rb_root *);
extern void ext2fs_rb_erase(struct rb_node *, struct rb_root *);

typedef void (*rb_augment_f)(struct rb_node *node, void *data);

extern void ext2fs_rb_augment_insert(struct rb_node *node,
			      rb_augment_f func, void *data);
extern struct rb_node *ext2fs_rb_augment_erase_begin(struct rb_node *node);
extern void ext2fs_rb_augment_erase_end(struct rb_node *node,
				 rb_augment_f func, void *data);

/* Find logical next and previous nodes in a tree */
extern struct rb_node *ext2fs_rb_next(struct rb_node *);
extern struct rb_node *ext2fs_rb_prev(struct rb_node *);
extern struct rb_node *ext2fs_rb_first(const struct rb_root *);
extern struct rb_node *ext2fs_rb_last(const struct rb_root *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
extern void ext2fs_rb_replace_node(struct rb_node *victim, struct rb_node *new,
				 struct rb_root *root);

static inline void ext2fs_rb_link_node(struct rb_node * node,
				     struct rb_node * parent,
				     struct rb_node ** rb_link)
{
	node->rb_parent_color = (unsigned long )parent;
	node->rb_left = node->rb_right = NULL;

	*rb_link = node;
}

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;
	struct rb_node *parent = ext2fs_rb_parent(node);

	if ((node->rb_right = right->rb_left))
		ext2fs_rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	ext2fs_rb_set_parent(right, parent);

	if (parent)
	{
		if (node == parent->rb_left)
			parent->rb_left = right;
		else
			parent->rb_right = right;
	}
	else
		root->rb_node = right;
	ext2fs_rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = ext2fs_rb_parent(node);

	if ((node->rb_left = left->rb_right))
		ext2fs_rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	ext2fs_rb_set_parent(left, parent);

	if (parent)
	{
		if (node == parent->rb_right)
			parent->rb_right = left;
		else
			parent->rb_left = left;
	}
	else
		root->rb_node = left;
	ext2fs_rb_set_parent(node, left);
}

void ext2fs_rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	while ((parent = ext2fs_rb_parent(node)) && ext2fs_rb_is_red(parent))
	{
		gparent = ext2fs_rb_parent(parent);

		if (parent == gparent->rb_left)
		{
			{
				register struct rb_node *uncle = gparent->rb_right;
				if (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_right == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
		} else {
			{
				register struct rb_node *uncle = gparent->rb_left;
				if (uncle && ext2fs_rb_is_red(uncle))
				{
					ext2fs_rb_set_black(uncle);
					ext2fs_rb_set_black(parent);
					ext2fs_rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			ext2fs_rb_set_black(parent);
			ext2fs_rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	ext2fs_rb_set_black(root->rb_node);
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
			     struct rb_root *root)
{
	struct rb_node *other;

	while ((!node || ext2fs_rb_is_black(node)) && node != root->rb_node)
	{
		if (parent->rb_left == node)
		{
			other = parent->rb_right;
			if (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			if ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			else
			{
				if (!other->rb_right || ext2fs_rb_is_black(other->rb_right))
				{
					ext2fs_rb_set_black(other->rb_left);
					ext2fs_rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_right);
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}
		else
		{
			other = parent->rb_left;
			if (ext2fs_rb_is_red(other))
			{
				ext2fs_rb_set_black(other);
				ext2fs_rb_set_red(parent);
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			if ((!other->rb_left || ext2fs_rb_is_black(other->rb_left)) &&
			    (!other->rb_right || ext2fs_rb_is_black(other->rb_right)))
			{
				ext2fs_rb_set_red(other);
				node = parent;
				parent = ext2fs_rb_parent(node);
			}
			else
			{
				if (!other->rb_left || ext2fs_rb_is_black(other->rb_left))
				{
					ext2fs_rb_set_black(other->rb_right);
					ext2fs_rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				ext2fs_rb_set_color(other, ext2fs_rb_color(parent));
				ext2fs_rb_set_black(parent);
				ext2fs_rb_set_black(other->rb_left);
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if (node)
		ext2fs_rb_set_black(node);
}

void ext2fs_rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

	if (!node->rb_left)
		child = node->rb_right;
	else if (!node->rb_right)
		child = node->rb_left;
	else
	{
		struct rb_node *old = node, *left;

		node = node->rb_right;
		while ((left = node->rb_left) != NULL)
			node = left;

		if (ext2fs_rb_parent(old)) {
			if (ext2fs_rb_parent(old)->rb_left == old)
				ext2fs_rb_parent(old)->rb_left = node;
			else
				ext2fs_rb_parent(old)->rb_right = node;
		} else
			root->rb_node = node;

		child = node->rb_right;
		parent = ext2fs_rb_parent(node);
		color = ext2fs_rb_color(node);

		if (parent == old) {
			parent = node;
		} else {
			if (child)
				ext2fs_rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			ext2fs_rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		ext2fs_rb_set_parent(old->rb_left, node);

		goto color;
	}

	parent = ext2fs_rb_parent(node);
	color = ext2fs_rb_color(node);

	if (child)
		ext2fs_rb_set_parent(child, parent);
	if (parent)
	{
		if (parent->rb_left == node)
			parent->rb_left = child;
		else
			parent->rb_right = child;
	}
	else
		root->rb_node = child;

 color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

static void ext2fs_rb_augment_path(struct rb_node *node, rb_augment_f func, void *data)
{
	struct rb_node *parent;

up:
	func(node, data);
	parent = ext2fs_rb_parent(node);
	if (!parent)
		return;

	if (node == parent->rb_left && parent->rb_right)
		func(parent->rb_right, data);
	else if (parent->rb_left)
		func(parent->rb_left, data);

	node = parent;
	goto up;
}

/*
 * after inserting @node into the tree, update the tree to account for
 * both the new entry and any damage done by rebalance
 */
void ext2fs_rb_augment_insert(struct rb_node *node, rb_augment_f func, void *data)
{
	if (node->rb_left)
		node = node->rb_left;
	else if (node->rb_right)
		node = node->rb_right;

	ext2fs_rb_augment_path(node, func, data);
}

/*
 * before removing the node, find the deepest node on the rebalance path
 * that will still be there after @node gets removed
 */
struct rb_node *ext2fs_rb_augment_erase_begin(struct rb_node *node)
{
	struct rb_node *deepest;

	if (!node->rb_right && !node->rb_left)
		deepest = ext2fs_rb_parent(node);
	else if (!node->rb_right)
		deepest = node->rb_left;
	else if (!node->rb_left)
		deepest = node->rb_right;
	else {
		deepest = ext2fs_rb_next(node);
		if (deepest->rb_right)
			deepest = deepest->rb_right;
		else if (ext2fs_rb_parent(deepest) != node)
			deepest = ext2fs_rb_parent(deepest);
	}

	return deepest;
}

/*
 * after removal, update the tree to account for the removed entry
 * and any rebalance damage.
 */
void ext2fs_rb_augment_erase_end(struct rb_node *node, rb_augment_f func, void *data)
{
	if (node)
		ext2fs_rb_augment_path(node, func, data);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_node *ext2fs_rb_first(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_left)
		n = n->rb_left;
	return n;
}

struct rb_node *ext2fs_rb_last(const struct rb_root *root)
{
	struct rb_node	*n;

	n = root->rb_node;
	if (!n)
		return NULL;
	while (n->rb_right)
		n = n->rb_right;
	return n;
}

struct rb_node *ext2fs_rb_next(struct rb_node *node)
{
	struct rb_node *parent;

	if (ext2fs_rb_parent(node) == node)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->rb_right) {
		node = node->rb_right;
		while (node->rb_left)
			node=node->rb_left;
		return (struct rb_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = ext2fs_rb_parent(node)) && node == parent->rb_right)
		node = parent;

	return parent;
}

struct rb_node *ext2fs_rb_prev(struct rb_node *node)
{
	struct rb_node *parent;

	if (ext2fs_rb_parent(node) == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->rb_left) {
		node = node->rb_left;
		while (node->rb_right)
			node=node->rb_right;
		return (struct rb_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = ext2fs_rb_parent(node)) && node == parent->rb_left)
		node = parent;

	return parent;
}

void ext2fs_rb_replace_node(struct rb_node *victim, struct rb_node *new,
			  struct rb_root *root)
{
	struct rb_node *parent = ext2fs_rb_parent(victim);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->rb_left)
			parent->rb_left = new;
		else
			parent->rb_right = new;
	} else {
		root->rb_node = new;
	}
	if (victim->rb_left)
		ext2fs_rb_set_parent(victim->rb_left, new);
	if (victim->rb_right)
		ext2fs_rb_set_parent(victim->rb_right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *victim;
}

struct bmap_rb_extent {
	struct rb_node node;
	__u64 start;
	__u64 count;
};

struct ext2fs_rb_private {
	struct rb_root root;
	struct bmap_rb_extent *wcursor;
	struct bmap_rb_extent *rcursor;
	struct bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

inline static struct bmap_rb_extent *node_to_extent(struct rb_node *node)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	return (struct bmap_rb_extent *) node;
}

static int rb_insert_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *);
static void rb_get_new_extent(struct bmap_rb_extent **, __u64, __u64);

//#define DEBUG_RB 

#ifdef DEBUG_RB
static void print_tree(struct rb_root *root)
{
	struct rb_node *node = NULL;
	struct bmap_rb_extent *ext;

	printf("\t\t\t=================================\n");
	node = ext2fs_rb_first(root);
	for (node = ext2fs_rb_first(root); node != NULL; 
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		printf("\t\t\t--> (%llu -> %llu)\n",
			ext->start, ext->start + ext->count);
	}
	printf("\t\t\t=================================\n");
}

static void check_tree(struct rb_root *root, const char *msg)
{
	struct rb_node *node;
	struct bmap_rb_extent *ext, *old = NULL;

	for (node = ext2fs_rb_first(root); node;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		if (ext->count == 0) {
			printf("Tree Error: count is zero\n");
			printf("extent: %llu -> %llu (%llu)\n", ext->start,
				ext->start + ext->count, ext->count);
			goto err_out;
		}
		if (ext->start + ext->count < ext->start) {
			printf("Tree Error: start or count is crazy\n");
			printf("extent: %llu -> %llu (%llu)\n", ext->start,
				ext->start + ext->count, ext->count);
			goto err_out;
		}

		if (old) {
			if (old->start > ext->start) {
				printf("Tree Error: start is crazy\n");
				printf("extent: %llu -> %llu (%llu)\n",
					old->start, old->start + old->count,
					old->count);
				printf("extent next: %llu -> %llu (%llu)\n",
					ext->start, ext->start + ext->count,
					ext->count);
				goto err_out;
			}
			if ((old->start + old->count) >= ext->start) {
				printf("Tree Error: extent is crazy\n");
				printf("extent: %llu -> %llu (%llu)\n",
					old->start, old->start + old->count,
					old->count);
				printf("extent next: %llu -> %llu (%llu)\n",
					ext->start, ext->start + ext->count,
					ext->count);
				goto err_out;
			}
		}
		old = ext;
	}
	return;

err_out:
	printf("%s\n", msg);
	print_tree(root);
	exit(1);
}
#else
#define check_tree(root, msg) do {} while (0)
#define print_tree(root) do {} while (0)
#endif

#define RB_ROOT	(struct rb_root) { NULL, }

static void rb_get_new_extent(struct bmap_rb_extent **ext, __u64 start,
			     __u64 count)
{
	struct bmap_rb_extent *new_ext;
	int retval;

	retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
				&new_ext);
	if (retval)
		abort();

	new_ext->start = start;
	new_ext->count = count;
	*ext = new_ext;
}

inline
static void rb_free_extent(struct ext2fs_rb_private *bp,
			   struct bmap_rb_extent *ext)
{
	if (bp->wcursor == ext)
		bp->wcursor = NULL;
	if (bp->rcursor == ext)
		bp->rcursor = NULL;
	if (bp->rcursor_next == ext)
		bp->rcursor_next = NULL;
	ext2fs_free_mem(&ext);
}

static errcode_t rb_alloc_private_data (ext2fs_generic_bitmap bitmap)
{
	struct ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof (struct ext2fs_rb_private), &bp);
	if (retval)
		return retval;

	bp->root = RB_ROOT;
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->private = (void *) bp;
	return 0;
}

static errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	if (retval)
		return retval;

	return 0;
}

static void rb_free_tree(struct rb_root *root)
{
	struct bmap_rb_extent *ext;
	struct rb_node *node, *next;

	for (node = ext2fs_rb_first(root); node; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		ext2fs_rb_erase(node, root);
		ext2fs_free_mem(&ext);
	}
}

static void rb_free_bmap(ext2fs_generic_bitmap bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	ext2fs_free_mem(&bp);
	bp = 0;
}

static errcode_t rb_copy_bmap(ext2fs_generic_bitmap src,
			      ext2fs_generic_bitmap dest)
{
	struct ext2fs_rb_private *src_bp, *dest_bp;
	struct bmap_rb_extent *src_ext, *dest_ext;
	struct rb_node *dest_node, *src_node, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	if (retval)
		return retval;

	src_bp = (struct ext2fs_rb_private *) src->private;
	dest_bp = (struct ext2fs_rb_private *) dest->private;
	src_bp->rcursor = NULL;
	dest_bp->rcursor = NULL;

	src_node = ext2fs_rb_first(&src_bp->root);
	while (src_node) {
		src_ext = node_to_extent(src_node);
		retval = ext2fs_get_mem(sizeof (struct bmap_rb_extent),
					&dest_ext);
		if (retval)
			break;

		memcpy(dest_ext, src_ext, sizeof(struct bmap_rb_extent));

		dest_node = &dest_ext->node;
		n = &dest_bp->root.rb_node;

		dest_last = NULL;
		if (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->root);
			n = &(dest_last)->rb_right;
		}

		ext2fs_rb_link_node(dest_node, dest_last, n);
		ext2fs_rb_insert_color(dest_node, &dest_bp->root);

		src_node = ext2fs_rb_next(src_node);
	}

	return retval;
}

static void rb_truncate(__u64 new_max, struct rb_root *root)
{
	struct bmap_rb_extent *ext;
	struct rb_node *node;

	node = ext2fs_rb_last(root);
	while (node) {
		ext = node_to_extent(node);

		if ((ext->start + ext->count - 1) <= new_max)
			break;
		else if (ext->start > new_max) {
			ext2fs_rb_erase(node, root);
			ext2fs_free_mem(&ext);
			node = ext2fs_rb_last(root);
			continue;
		} else
			ext->count = new_max - ext->start + 1;
	}
}

static errcode_t rb_resize_bmap(ext2fs_generic_bitmap bmap,
				__u64 new_end, __u64 new_real_end)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bmap->private;
	bp->rcursor = NULL;
	bp->wcursor = NULL;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->root);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	if (bmap->end < bmap->real_end)
		rb_insert_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	return 0;

}

inline static int
rb_test_bit(struct ext2fs_rb_private *bp, __u64 bit)
{
	struct bmap_rb_extent *rcursor, *next_ext = NULL;
	struct rb_node *parent = NULL, *next;
	struct rb_node **n = &bp->root.rb_node;
	struct bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	if (!rcursor)
		goto search_tree;

	if (bit >= rcursor->start && bit < rcursor->start + rcursor->count) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		return 1;
	}

	next_ext = bp->rcursor_next;
	if (!next_ext) {
		next = ext2fs_rb_next(&rcursor->node);
		if (next)
			next_ext = node_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	if (next_ext) {
		if ((bit >= rcursor->start + rcursor->count) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			return 0;
		}
	}
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;

	rcursor = bp->wcursor;
	if (!rcursor)
		goto search_tree;

	if (bit >= rcursor->start && bit < rcursor->start + rcursor->count)
		return 1;

search_tree:

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (bit < ext->start)
			n = &(*n)->rb_left;
		else if (bit >= (ext->start + ext->count))
			n = &(*n)->rb_right;
		else {
			bp->rcursor = ext;
			bp->rcursor_next = NULL;
			return 1;
		}
	}
	return 0;
}

static int rb_insert_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *bp)
{
	struct rb_root *root = &bp->root;
	struct rb_node *parent = NULL, **n = &root->rb_node;
	struct rb_node *new_node, *node, *next;
	struct bmap_rb_extent *new_ext;
	struct bmap_rb_extent *ext;
	int retval = 0;

	bp->rcursor_next = NULL;
	ext = bp->wcursor;
	if (ext) {
		if (start >= ext->start &&
		    start <= (ext->start + ext->count)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			goto got_extent;
		}
	}

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);

		if (start < ext->start) {
			n = &(*n)->rb_left;
		} else if (start > (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} else {
got_extent:
			if ((start + count) <= (ext->start + ext->count))
				return 1;

			if ((ext->start + ext->count) == start)
				retval = 0;
			else
				retval = 1;

			count += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_node = &ext->node;

			goto skip_insert;
		}
	}

	rb_get_new_extent(&new_ext, start, count);

	new_node = &new_ext->node;
	ext2fs_rb_link_node(new_node, parent, n);
	ext2fs_rb_insert_color(new_node, root);
	bp->wcursor = new_ext;

	node = ext2fs_rb_prev(new_node);
	if (node) {
		ext = node_to_extent(node);
		if ((ext->start + ext->count) == start) {
			start = ext->start;
			count += ext->count;
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
		}
	}

skip_insert:
	/* See if we can merge extent to the right */
	for (node = ext2fs_rb_next(new_node); node != NULL; node = next) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);

		if ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		if ((start + count) < ext->start)
			break;

		/* ext is embedded in new_ext interval */
		if ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			continue;
		} else {
		/* merge ext with new_ext */
			count += ((ext->start + ext->count) -
				  (start + count));
			ext2fs_rb_erase(node, root);
			rb_free_extent(bp, ext);
			break;
		}
	}

	new_ext->start = start;
	new_ext->count = count;

	return retval;
}

static int rb_remove_extent(__u64 start, __u64 count,
			    struct ext2fs_rb_private *bp)
{
	struct rb_root *root = &bp->root;
	struct rb_node *parent = NULL, **n = &root->rb_node;
	struct rb_node *node;
	struct bmap_rb_extent *ext;
	__u64 new_start, new_count;
	int retval = 0;

	if (ext2fs_rb_empty_root(root))
		return 0;

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_left;
			continue;
		} else if (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
			continue;
		}

		if ((start > ext->start) &&
		    (start + count) < (ext->start + ext->count)) {
			/* We have to split extent into two */
			new_start = start + count;
			new_count = (ext->start + ext->count) - new_start;

			ext->count = start - ext->start;

			rb_insert_extent(new_start, new_count, bp);
			return 1;
		}

		if ((start + count) >= (ext->start + ext->count)) {
			ext->count = start - ext->start;
			retval = 1;
		}

		if (0 == ext->count) {
			parent = ext2fs_rb_next(&ext->node);
			ext2fs_rb_erase(&ext->node, root);
			rb_free_extent(bp, ext);
			break;
		}

		if (start == ext->start) {
			ext->start += count;
			ext->count -= count;
			return 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	for (; parent != NULL; parent = node) {
		node = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);
		if ((ext->start + ext->count) <= start)
			continue;

		/* No more extents to be removed/truncated */
		if ((start + count) < ext->start)
			break;

		/* The entire extent is within the region to be removed */
		if ((start + count) >= (ext->start + ext->count)) {
			ext2fs_rb_erase(parent, root);
			rb_free_extent(bp, ext);
			retval = 1;
			continue;
		} else {
			/* modify the last extent in reigon to be removed */
			ext->count -= ((start + count) - ext->start);
			ext->start = start + count;
			retval = 1;
			break;
		}
	}

	return retval;
}

static int rb_mark_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	int retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_insert_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);
	return retval;
}

static int rb_unmark_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;
	int retval;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_tree(&bp->root, __func__);

	return retval;
}

inline
static int rb_test_bmap(ext2fs_generic_bitmap bitmap, __u64 arg)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	return rb_test_bit(bp, arg);
}

static void rb_mark_bmap_extent(ext2fs_generic_bitmap bitmap, __u64 arg,
				unsigned int num)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_insert_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static void rb_unmark_bmap_extent(ext2fs_generic_bitmap bitmap, __u64 arg,
				  unsigned int num)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	arg -= bitmap->start;

	rb_remove_extent(arg, num, bp);
	check_tree(&bp->root, __func__);
}

static int rb_test_clear_bmap_extent(ext2fs_generic_bitmap bitmap,
				     __u64 start, unsigned int len)
{
	struct rb_node *parent = NULL, **n;
	struct rb_node *node, *next;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	int retval = 1;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	if (len == 0 || ext2fs_rb_empty_root(&bp->root))
		return 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_left;
		} else if (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} else {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			return 0;
		}
	}

	node = parent;
	while (node) {
		next = ext2fs_rb_next(node);
		ext = node_to_extent(node);
		node = next;

		if ((ext->start + ext->count) <= start)
			continue;

		/* No more merging */
		if ((start + len) <= ext->start)
			break;

		retval = 0;
		break;
	}
	return retval;
}

int ext2fs_test_bit(unsigned int nr, const void * addr)
{
	int			mask;
	const unsigned char	*ADDR = (const unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	return (mask & *ADDR);
}

static errcode_t rb_set_bmap_range(ext2fs_generic_bitmap bitmap,
				     __u64 start, size_t num, void *in)
{
	struct ext2fs_rb_private *bp;
	unsigned char *cp = in;
	size_t i;
	int first_set = -1;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	for (i = 0; i < num; i++) {
		if ((i & 7) == 0) {
			unsigned char c = cp[i/8];
			if (c == 0xFF) {
				if (first_set == -1)
					first_set = i;
				i += 7;
				continue;
			}
			if ((c == 0x00) && (first_set == -1)) {
				i += 7;
				continue;
			}
		}
		if (ext2fs_test_bit(i, in)) {
			if (first_set == -1)
				first_set = i;
			continue;
		}
		if (first_set == -1)
			continue;

		rb_insert_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_tree(&bp->root, __func__);
		first_set = -1;
	}
	if (first_set != -1) {
		rb_insert_extent(start + first_set - bitmap->start,
				 num - first_set, bp);
		check_tree(&bp->root, __func__);
	}

	return 0;
}

static errcode_t rb_get_bmap_range(ext2fs_generic_bitmap bitmap,
				     __u64 start, size_t num, void *out)
{

	struct rb_node *parent = NULL, *next, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;
	__u64 count, pos;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;

	if (ext2fs_rb_empty_root(&bp->root))
		return 0;

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_left;
		} else if (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} else
			break;
	}

	memset(out, 0, (num + 7) >> 3);

	for (; parent != NULL; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = node_to_extent(parent);

		pos = ext->start;
		count = ext->count;
		if (pos >= start + num)
			break;
		if (pos < start) {
			if (pos + count <  start)
				continue;
			count -= start - pos;
			pos = start;
		}
		if (pos + count > start + num)
			count = start + num - pos;

		while (count > 0) {
			if ((count >= 8) &&
			    ((pos - start) % 8) == 0) {
				int nbytes = count >> 3;
				int offset = (pos - start) >> 3;

				memset(((char *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				count -= nbytes << 3;
				continue;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			count--;
		}
	}
	return 0;
}

static void rb_clear_bmap(ext2fs_generic_bitmap bitmap)
{
	struct ext2fs_rb_private *bp;

	bp = (struct ext2fs_rb_private *) bitmap->private;

	rb_free_tree(&bp->root);
	bp->rcursor = NULL;
	bp->rcursor_next = NULL;
	bp->wcursor = NULL;
	check_tree(&bp->root, __func__);
}

static errcode_t rb_find_first_zero(ext2fs_generic_bitmap bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_node *parent = NULL, **n;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	if (start > end)
		return EINVAL;

	if (ext2fs_rb_empty_root(&bp->root))
		return ENOENT;

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_left;
		} else if (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} else if (ext->start + ext->count <= end) {
			*out = ext->start + ext->count + bitmap->start;
			return 0;
		} else
			return ENOENT;
	}

	*out = start + bitmap->start;
	return 0;
}

static errcode_t rb_find_first_set(ext2fs_generic_bitmap bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	struct rb_node *parent = NULL, **n;
	struct rb_node *node;
	struct ext2fs_rb_private *bp;
	struct bmap_rb_extent *ext;

	bp = (struct ext2fs_rb_private *) bitmap->private;
	n = &bp->root.rb_node;
	start -= bitmap->start;
	end -= bitmap->start;

	if (start > end)
		return EINVAL;

	if (ext2fs_rb_empty_root(&bp->root))
		return ENOENT;

	while (*n) {
		parent = *n;
		ext = node_to_extent(parent);
		if (start < ext->start) {
			n = &(*n)->rb_left;
		} else if (start >= (ext->start + ext->count)) {
			n = &(*n)->rb_right;
		} else {
			/* The start bit is set */
			*out = start + bitmap->start;
			return 0;
		}
	}

	node = parent;
	ext = node_to_extent(node);
	if (ext->start < start) {
		node = ext2fs_rb_next(node);
		if (node == NULL)
			return ENOENT;
		ext = node_to_extent(node);
	}
	if (ext->start <= end) {
		*out = ext->start + bitmap->start;
		return 0;
	}
	return ENOENT;
}

#ifdef ENABLE_BMAP_STATS
static void rb_print_stats(ext2fs_generic_bitmap bitmap)
{
	struct ext2fs_rb_private *bp;
	struct rb_node *node = NULL;
	struct bmap_rb_extent *ext;
	__u64 count = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	double eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	double m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (struct ext2fs_rb_private *) bitmap->private;

	for (node = ext2fs_rb_first(&bp->root); node != NULL;
	     node = ext2fs_rb_next(node)) {
		ext = node_to_extent(node);
		count++;
		if (ext->count > max_size)
			max_size = ext->count;
		if (ext->count < min_size)
			min_size = ext->count;
		size += ext->count;
	}

	if (count)
		avg_size = size / count;
	if (min_size == ULONG_MAX)
		min_size = 0;
	eff = (double)((count * sizeof(struct bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_count + bitmap->stats.mark_ext_count;
	test_all = bitmap->stats.test_count + bitmap->stats.test_ext_count;
	if (mark_all)
		m_hit = ((double)bp->mark_hit / mark_all) * 100;
	if (test_all)
		t_hit = ((double)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		count, ((count * sizeof(struct bmap_rb_extent)) +
			sizeof(struct ext2fs_rb_private)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		min_size);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		max_size, avg_size);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n", size,
		bitmap->real_end - bitmap->start);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#else
static void rb_print_stats(ext2fs_generic_bitmap bitmap EXT2FS_ATTR((unused)))
{
}
#endif

struct ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
	.type = EXT2FS_BMAP64_RBTREE,
	.new_bmap = rb_new_bmap,
	.free_bmap = rb_free_bmap,
	.copy_bmap = rb_copy_bmap,
	.resize_bmap = rb_resize_bmap,
	.mark_bmap = rb_mark_bmap,
	.unmark_bmap = rb_unmark_bmap,
	.test_bmap = rb_test_bmap,
	.test_clear_bmap_extent = rb_test_clear_bmap_extent,
	.mark_bmap_extent = rb_mark_bmap_extent,
	.unmark_bmap_extent = rb_unmark_bmap_extent,
	.set_bmap_range = rb_set_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};

errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    int type, __u64 start, __u64 end,
				    __u64 real_end,
				    const char *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap	bitmap;
	struct ext2_bitmap_ops	*ops;
	ext2_ino_t num_dirs;
	errcode_t retval;

	if (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	switch (type) {
	case EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		break;
	case EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		break;
	/*case EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_num_dirs(fs, &num_dirs);
		if (retval || num_dirs > (fs->super->s_inodes_count / 320))
			ops = &ext2fs_blkmap64_bitarray;
		else
			ops = &ext2fs_blkmap64_rbtree;
		break;
	*/default:
		return EINVAL;
	}

	retval = ext2fs_get_memzero(sizeof(struct ext2fs_struct_generic_bitmap),
				    &bitmap);
	if (retval)
		return retval;

#ifdef ENABLE_BMAP_STATS
	if (gettimeofday(&bitmap->stats.created,
			 (struct timezone *) NULL) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		return 1;
	}
	bitmap->stats.type = type;
#endif

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_bits = 0;
	switch (magic) {
	case EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		break;
	case EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		break;
	default:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	if (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		if (retval) {
			ext2fs_free_mem(&bitmap);
			return retval;
		}
		strcpy(bitmap->description, descr);
	} else
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	if (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		return retval;
	}

	*ret = bitmap;
	return 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
int ext2fs_mem_is_zero(const char *mem, size_t len)
{
	static const char zero_buf[256];

	while (len >= sizeof(zero_buf)) {
		if (memcmp(mem, zero_buf, sizeof(zero_buf)))
			return 0;
		len -= sizeof(zero_buf);
		mem += sizeof(zero_buf);
	}
	/* Deal with leftover bytes. */
	if (len)
		return !memcmp(mem, zero_buf, len);
	return 1;
}

#ifdef ENABLE_BMAP_STATS_OPS
#define INC_STAT(map, name) map->stats.name
#else
#define INC_STAT(map, name) ;;
#endif

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       const char *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		start, end, real_end;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	//fs->write_bitmaps = ext2fs_write_bitmaps;

	start = 1;
	end = fs->super->s_inodes_count;
	real_end = (EXT2_INODES_PER_GROUP(fs->super) * fs->group_desc_count);

	return (ext2fs_alloc_generic_bmap(fs,
			EXT2_ET_MAGIC_INODE_BITMAP64,
			fs->default_bitmap_type,
			start, end, real_end, descr, ret));

}

errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
				       const char *descr,
				       ext2fs_block_bitmap *ret)
{
	__u64		start, end, real_end;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	//fs->write_bitmaps = ext2fs_write_bitmaps;

	start = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	end = EXT2FS_B2C(fs, ext2fs_blocks_count(fs->super)-1);
	real_end = ((__u64) EXT2_CLUSTERS_PER_GROUP(fs->super)
		    * (__u64) fs->group_desc_count)-1 + start;

	return (ext2fs_alloc_generic_bmap(fs,
			EXT2_ET_MAGIC_BLOCK_BITMAP64,
			fs->default_bitmap_type,
			start, end, real_end, descr, ret));

}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap bmap,
					__u64 start, unsigned int num,
					void *in)
{
	if (!bmap)
		return EINVAL;

	if (!EXT2FS_IS_64_BITMAP(bmap))
		return EINVAL;

	INC_STAT(bmap, set_range_count);

	return bmap->bitmap_ops->set_bmap_range(bmap, start, num, in);
}

errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t num,
					 void *in)
{
	return (ext2fs_set_generic_bmap_range(bmap, start, num, in));
}


errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t num,
					 void *in)
{
	return (ext2fs_set_generic_bmap_range(bmap, start, num, in));
}

#define IO_FLAG_RW		0x0001
#define IO_FLAG_EXCLUSIVE	0x0002
#define IO_FLAG_DIRECT_IO	0x0004

/*
 * Convenience functions....
 */
#define io_channel_close(c) 		((c)->manager->close((c)))
#define io_channel_set_blksize(c,s)	((c)->manager->set_blksize((c),s))
#define io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#define io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#define io_channel_flush(c) 		((c)->manager->flush((c)))
#define io_channel_bumpcount(c)		((c)->refcount++)


errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, void *buf)
{
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	blk_t		*block_nr;
	int		i;
	int		limit = fs->blocksize >> 2;
#endif

	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	if (retval)
		return retval;
#ifdef WORDS_BIGENDIAN
	block_nr = (blk_t *) buf;
	for (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	return 0;
}

ext2_filsys current_fs;
ext2_ino_t	root, cwd;

struct ext2_struct_u32_list {
	int	magic;
	int	num;
	int	size;
	__u32	*list;
	int	badblocks_flags;
};

typedef struct ext2_struct_u32_list *ext2_u32_list;

struct ext2_struct_u32_iterate {
	int			magic;
	ext2_u32_list		bb;
	int			ptr;
};

typedef struct ext2_struct_u32_iterate *ext2_badblocks_iterate;
typedef struct ext2_struct_u32_iterate *ext2_u32_iterate;

/*
 * Directory block iterator definition
 */
struct ext2_struct_dblist {
	int			magic;
	ext2_filsys		fs;
	unsigned long long	size;
	unsigned long long	count;
	int			sorted;
	struct ext2_db_entry2 *	list;
};

/* inode.c */
extern errcode_t ext2fs_create_inode_cache(ext2_filsys fs,
					   unsigned int cache_size);
extern void ext2fs_free_inode_cache(struct ext2_inode_cache *icache);
extern errcode_t ext2fs_flush_icache(ext2_filsys fs);
#define EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS	8

extern errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
					struct ext2_inode * inode,
					int bufsize);
extern errcode_t ext2fs_read_inode (ext2_filsys fs, ext2_ino_t ino,
			    struct ext2_inode * inode);

#if defined(__linux__) && defined(_IO) && !defined(BLKROGET)
#define BLKROGET   _IO(0x12, 94) /* Get read-only status (0 = read_write).  */
#endif

#undef ALIGN_DEBUG

struct unix_cache {
	char			*buf;
	unsigned long long	block;
	int			access_time;
	unsigned		dirty:1;
	unsigned		in_use:1;
};

#define CACHE_SIZE 8
#define WRITE_DIRECT_SIZE 4	/* Must be smaller than CACHE_SIZE */
#define READ_DIRECT_SIZE 4	/* Should be smaller than CACHE_SIZE */

struct unix_private_data {
	int	magic;
	int	dev;
	int	flags;
	int	align;
	int	access_time;
	ext2_loff_t offset;
	struct unix_cache cache[CACHE_SIZE];
	void	*bounce;
	struct struct_io_stats io_stats;
};

#define IS_ALIGNED(n, align) ((((unsigned long) n) & \
			       ((unsigned long) ((align)-1))) == 0)

static errcode_t unix_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;

	struct unix_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	if (stats)
		*stats = &data->io_stats;

	return retval;
}

#if defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)

#define my_llseek lseek64

#else
#if defined(HAVE_LLSEEK)
#include <sys/syscall.h>

#ifndef HAVE_LLSEEK_PROTOTYPE
extern long long llseek (int fd, long long offset, int origin);
#endif

#define my_llseek llseek

#else	/* ! HAVE_LLSEEK */

#if SIZEOF_LONG == SIZEOF_LONG_LONG

#define llseek lseek

#else /* SIZEOF_LONG != SIZEOF_LONG_LONG */

#include <linux/unistd.h>

#ifndef __NR__llseek
#define __NR__llseek            140
#endif

#ifndef __i386__
static int _llseek (unsigned int, unsigned long,
		   unsigned long, ext2_loff_t *, unsigned int);

static _syscall5(int,_llseek,unsigned int,fd,unsigned long,offset_high,
		 unsigned long, offset_low,ext2_loff_t *,result,
		 unsigned int, origin)
#endif

static ext2_loff_t my_llseek (int fd, ext2_loff_t offset, int origin)
{
	ext2_loff_t result;
	int retval;

#ifndef __i386__
	retval = _llseek(fd, ((unsigned long long) offset) >> 32,
#else
	retval = syscall(__NR__llseek, fd, (unsigned long long) (offset >> 32),
#endif
			  ((unsigned long long) offset) & 0xffffffff,
			&result, origin);
	return (retval == -1 ? (ext2_loff_t) retval : result);
}

#endif	/* __alpha__ || __ia64__ */

#endif /* HAVE_LLSEEK */
#endif /* defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE) */

ext2_loff_t ext2fs_llseek (int fd, ext2_loff_t offset, int origin)
{
#if SIZEOF_OFF_T >= SIZEOF_LONG_LONG
	return lseek (fd, offset, origin);
#else
	ext2_loff_t result;
	static int do_compat = 0;

	if (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	if (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		if (offset < ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))
			return lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		return -1;
	}
	return result;
#endif
}

/*
 * Here are the raw I/O functions
 */
static errcode_t raw_read_blk(io_channel channel,
			      struct unix_private_data *data,
			      unsigned long long block,
			      int count, void *bufv)
{
	errcode_t	retval;
	ssize_t		size;
	ext2_loff_t	location;
	int		actual = 0;
	unsigned char	*buf = bufv;

	size = (count < 0) ? -count : count * channel->block_size;
	data->io_stats.bytes_read += size;
	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

#ifdef HAVE_PREAD64
	/* Try an aligned pread */
	if ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pread64(data->dev, buf, size, location);
		if (actual == size)
			return 0;
	}
#elif HAVE_PREAD
	/* Try an aligned pread */
	if ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pread(data->dev, buf, size, location);
		if (actual == size)
			return 0;
	}
#endif /* HAVE_PREAD */

	if (ext2fs_llseek(data->dev, location, SEEK_SET) != location) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		goto error_out;
	}
	if ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = read(data->dev, buf, size);
		if (actual != size) {
		short_read:
			if (actual < 0)
				actual = 0;
			retval = EXT2_ET_SHORT_READ;
			goto error_out;
		}
		return 0;
	}

#ifdef ALIGN_DEBUG
	printf("raw_read_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) size);
#endif

	/*
	 * The buffer or size which we're trying to read isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
	while (size > 0) {
		actual = read(data->dev, data->bounce, channel->block_size);
		if (actual != channel->block_size)
			goto short_read;
		actual = size;
		if (size > channel->block_size)
			actual = channel->block_size;
		memcpy(buf, data->bounce, actual);
		size -= actual;
		buf += actual;
	}
	return 0;

error_out:
	memset((char *) buf+actual, 0, size-actual);
	if (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	return retval;
}

static errcode_t raw_write_blk(io_channel channel,
			       struct unix_private_data *data,
			       unsigned long long block,
			       int count, const void *bufv)
{
	ssize_t		size;
	ext2_loff_t	location;
	int		actual = 0;
	errcode_t	retval;
	const unsigned char *buf = bufv;

	if (count == 1)
		size = channel->block_size;
	else {
		if (count < 0)
			size = -count;
		else
			size = count * channel->block_size;
	}
	data->io_stats.bytes_written += size;

	location = ((ext2_loff_t) block * channel->block_size) + data->offset;

#ifdef HAVE_PWRITE64
	/* Try an aligned pwrite */
	if ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = pwrite64(data->dev, buf, size, location);
		if (actual == size)
			return 0;
	}
#elif HAVE_PWRITE
	/* Try an aligned pwrite */
	if ((sizeof(off_t) >= sizeof(ext2_loff_t)) &&
	    ((channel->align == 0) ||
	     (IS_ALIGNED(buf, channel->align) &&
	      IS_ALIGNED(size, channel->align)))) {
		actual = pwrite(data->dev, buf, size, location);
		if (actual == size)
			return 0;
	}
#endif /* HAVE_PWRITE */

	if (ext2fs_llseek(data->dev, location, SEEK_SET) != location) {
		retval = errno ? errno : EXT2_ET_LLSEEK_FAILED;
		goto error_out;
	}

	if ((channel->align == 0) ||
	    (IS_ALIGNED(buf, channel->align) &&
	     IS_ALIGNED(size, channel->align))) {
		actual = write(data->dev, buf, size);
		if (actual != size) {
		short_write:
			retval = EXT2_ET_SHORT_WRITE;
			goto error_out;
		}
		return 0;
	}

#ifdef ALIGN_DEBUG
	printf("raw_write_blk: O_DIRECT fallback: %p %lu\n", buf,
	       (unsigned long) size);
#endif
	/*
	 * The buffer or size which we're trying to write isn't aligned
	 * to the O_DIRECT rules, so we need to do this the hard way...
	 */
	while (size > 0) {
		if (size < channel->block_size) {
			actual = read(data->dev, data->bounce,
				      channel->block_size);
			if (actual != channel->block_size) {
				retval = EXT2_ET_SHORT_READ;
				goto error_out;
			}
		}
		actual = size;
		if (size > channel->block_size)
			actual = channel->block_size;
		memcpy(data->bounce, buf, actual);
		actual = write(data->dev, data->bounce, channel->block_size);
		if (actual != channel->block_size)
			goto short_write;
		size -= actual;
		buf += actual;
	}
	return 0;

error_out:
	if (channel->write_error)
		retval = (channel->write_error)(channel, block, count, buf,
						size, actual, retval);
	return retval;
}


extern errcode_t io_channel_alloc_buf(io_channel channel,
                                      int count, void *ptr);
/*
 * Here we implement the cache functions
 */

/* Allocate the cache buffers */
static errcode_t alloc_cache(io_channel channel,
			     struct unix_private_data *data)
{
	errcode_t		retval;
	struct unix_cache	*cache;
	int			i;

	data->access_time = 0;
	for (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		if (cache->buf)
			ext2fs_free_mem(&cache->buf);
		retval = io_channel_alloc_buf(channel, 0, &cache->buf);
		if (retval)
			return retval;
	}
	if (channel->align) {
		if (data->bounce)
			ext2fs_free_mem(&data->bounce);
		retval = io_channel_alloc_buf(channel, 0, &data->bounce);
	}
	return retval;
}

/* Free the cache buffers */
static void free_cache(struct unix_private_data *data)
{
	struct unix_cache	*cache;
	int			i;

	data->access_time = 0;
	for (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		cache->block = 0;
		cache->access_time = 0;
		cache->dirty = 0;
		cache->in_use = 0;
		if (cache->buf)
			ext2fs_free_mem(&cache->buf);
	}
	if (data->bounce)
		ext2fs_free_mem(&data->bounce);
}

#ifndef NO_IO_CACHE
/*
 * Try to find a block in the cache.  If the block is not found, and
 * eldest is a non-zero pointer, then fill in eldest with the cache
 * entry to that should be reused.
 */
static struct unix_cache *find_cached_block(struct unix_private_data *data,
					    unsigned long long block,
					    struct unix_cache **eldest)
{
	struct unix_cache	*cache, *unused_cache, *oldest_cache;
	int			i;

	unused_cache = oldest_cache = 0;
	for (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		if (!cache->in_use) {
			if (!unused_cache)
				unused_cache = cache;
			continue;
		}
		if (cache->block == block) {
			cache->access_time = ++data->access_time;
			return cache;
		}
		if (!oldest_cache ||
		    (cache->access_time < oldest_cache->access_time))
			oldest_cache = cache;
	}
	if (eldest)
		*eldest = (unused_cache) ? unused_cache : oldest_cache;
	return 0;
}

/*
 * Reuse a particular cache entry for another block.
 */
static void reuse_cache(io_channel channel, struct unix_private_data *data,
		 struct unix_cache *cache, unsigned long long block)
{
	if (cache->dirty && cache->in_use)
		raw_write_blk(channel, data, cache->block, 1, cache->buf);

	cache->in_use = 1;
	cache->dirty = 0;
	cache->block = block;
	cache->access_time = ++data->access_time;
}

/*
 * Flush all of the blocks in the cache
 */
static errcode_t flush_cached_blocks(io_channel channel,
				     struct unix_private_data *data,
				     int invalidate)

{
	struct unix_cache	*cache;
	errcode_t		retval, retval2;
	int			i;

	retval2 = 0;
	for (i=0, cache = data->cache; i < CACHE_SIZE; i++, cache++) {
		if (!cache->in_use)
			continue;

		if (invalidate)
			cache->in_use = 0;

		if (!cache->dirty)
			continue;

		retval = raw_write_blk(channel, data,
				       cache->block, 1, cache->buf);
		if (retval)
			retval2 = retval;
		else
			cache->dirty = 0;
	}
	return retval2;
}
#endif /* NO_IO_CACHE */

#ifdef __linux__
#ifndef BLKDISCARDZEROES
#define BLKDISCARDZEROES _IO(0x12,124)
#endif
#endif

int ext2fs_open_file(const char *pathname, int flags, mode_t mode)
{
	if (mode)
#if defined(HAVE_OPEN64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
		return open64(pathname, flags, mode);
	else
		return open64(pathname, flags);
#else
		return open(pathname, flags, mode);
	else
		return open(pathname, flags);
#endif
}

int ext2fs_stat(const char *path, ext2fs_struct_stat *buf)
{
#if defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	return stat64(path, buf);
#else
	return stat(path, buf);
#endif
}

int ext2fs_fstat(int fd, ext2fs_struct_stat *buf)
{
#if defined(HAVE_FSTAT64) && !defined(__OSX_AVAILABLE_BUT_DEPRECATED)
	return fstat64(fd, buf);
#else
	return fstat(fd, buf);
#endif
}

#define RLIMIT_FSIZE 1
 
static errcode_t unix_open(const char *name, int flags, io_channel *channel);
static errcode_t unix_close(io_channel channel);
static errcode_t unix_set_blksize(io_channel channel, int blksize);
static errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       int count, void *buf);
static errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       int count, void *buf);

static errcode_t unix_get_stats(io_channel channel, io_stats *stats);

static struct struct_io_manager struct_unix_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Unix I/O Manager",
	.open		= unix_open,
	.close		= unix_close,
	.set_blksize	= unix_set_blksize,
	.read_blk	= unix_read_blk,
	//.write_blk	= unix_write_blk,
	//.flush		= unix_flush,
	//.write_byte	= unix_write_byte,
	//.set_option	= unix_set_option,
	.get_stats	= unix_get_stats,
	.read_blk64	= unix_read_blk64,
	//.write_blk64	= unix_write_blk64,
	//.discard	= unix_discard,
	//.cache_readahead	= unix_cache_readahead,
	//.zeroout	= unix_zeroout,
};

io_manager unix_io_manager = &struct_unix_manager;

static errcode_t unix_open(const char *name, int flags, io_channel *channel)
{
	io_channel	io = NULL;
	struct unix_private_data *data = NULL;
	errcode_t	retval;
	int		open_flags;
	int		f_nocache = 0;
	ext2fs_struct_stat st;
#ifdef __linux__
	struct		utsname ut;
#endif

	if (name == 0)
		return EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(sizeof(struct struct_io_channel), &io);
	if (retval)
		goto cleanup;
	memset(io, 0, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(sizeof(struct unix_private_data), &data);
	if (retval)
		goto cleanup;

	io->manager = unix_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	if (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, sizeof(struct unix_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->io_stats.num_fields = 2;
	data->dev = -1;

	open_flags = (flags & IO_FLAG_RW) ? O_RDWR : O_RDONLY;
	if (flags & IO_FLAG_EXCLUSIVE)
		open_flags |= O_EXCL;
#if defined(O_DIRECT)
	if (flags & IO_FLAG_DIRECT_IO) {
		open_flags |= O_DIRECT;
		io->align = ext2fs_get_dio_alignment(data->dev);
	}
#elif defined(F_NOCACHE)
	if (flags & IO_FLAG_DIRECT_IO) {
		f_nocache = F_NOCACHE;
		io->align = 4096;
	}
#endif
	data->flags = flags;

	data->dev = ext2fs_open_file(io->name, open_flags, 0);
	if (data->dev < 0) {
		retval = errno;
		goto cleanup;
	}
	if (f_nocache) {
		if (fcntl(data->dev, f_nocache, 1) < 0) {
			retval = errno;
			goto cleanup;
		}
	}

	/*
	 * If the device is really a block device, then set the
	 * appropriate flag, otherwise we can set DISCARD_ZEROES flag
	 * because we are going to use punch hole instead of discard
	 * and if it succeed, subsequent read from sparse area returns
	 * zero.
	 */
	if (ext2fs_stat(io->name, &st) == 0) {
		if (S_ISBLK(st.st_mode))
			io->flags |= CHANNEL_FLAGS_BLOCK_DEVICE;
		else
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}

#ifdef BLKDISCARDZEROES
	{
		int zeroes = 0;
		if (ioctl(data->dev, BLKDISCARDZEROES, &zeroes) == 0 &&
		    zeroes)
			io->flags |= CHANNEL_FLAGS_DISCARD_ZEROES;
	}
#endif

#if defined(__CYGWIN__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	/*
	 * Some operating systems require that the buffers be aligned,
	 * regardless of O_DIRECT
	 */
	if (!io->align)
		io->align = 512;
#endif


	if ((retval = alloc_cache(io, data)))
		goto cleanup;

#ifdef BLKROGET
	if (flags & IO_FLAG_RW) {
		int error;
		int readonly = 0;

		/* Is the block device actually writable? */
		error = ioctl(data->dev, BLKROGET, &readonly);
		if (!error && readonly) {
			retval = EPERM;
			goto cleanup;
		}
	}
#endif

#ifdef __linux__
#undef RLIM_INFINITY
#if (defined(__alpha__) || ((defined(__sparc__) || defined(__mips__)) && (SIZEOF_LONG == 4)))
#define RLIM_INFINITY	((unsigned long)(~0UL>>1))
#else
#define RLIM_INFINITY  (~0UL)
#endif
	/*
	 * Work around a bug in 2.4.10-2.4.18 kernels where writes to
	 * block devices are wrongly getting hit by the filesize
	 * limit.  This workaround isn't perfect, since it won't work
	 * if glibc wasn't built against 2.2 header files.  (Sigh.)
	 *
	 */
	if ((flags & IO_FLAG_RW) &&
	    (uname(&ut) == 0) &&
	    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
	     (ut.release[2] == '4') && (ut.release[3] == '.') &&
	     (ut.release[4] == '1') && (ut.release[5] >= '0') &&
	     (ut.release[5] < '8')) &&
	    (ext2fs_stat(io->name, &st) == 0) &&
	    (S_ISBLK(st.st_mode))) {
		struct rlimit	rlim;

		rlim.rlim_cur = rlim.rlim_max = (unsigned long) RLIM_INFINITY;
		setrlimit(RLIMIT_FSIZE, &rlim);
		getrlimit(RLIMIT_FSIZE, &rlim);
		if (((unsigned long) rlim.rlim_cur) <
		    ((unsigned long) rlim.rlim_max)) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_FSIZE, &rlim);
		}
	}
#endif
	*channel = io;
	return 0;

cleanup:
	if (data) {
		if (data->dev >= 0)
			close(data->dev);
		free_cache(data);
		ext2fs_free_mem(&data);
	}
	if (io) {
		if (io->name) {
			ext2fs_free_mem(&io->name);
		}
		ext2fs_free_mem(&io);
	}
	return retval;
}

static errcode_t unix_close(io_channel channel)
{
	struct unix_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	if (--channel->refcount > 0)
		return 0;

#ifndef NO_IO_CACHE
	retval = flush_cached_blocks(channel, data, 0);
#endif

	if (close(data->dev) < 0)
		retval = errno;
	free_cache(data);

	ext2fs_free_mem(&channel->private_data);
	if (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);
	return retval;
}

static errcode_t unix_set_blksize(io_channel channel, int blksize)
{
	struct unix_private_data *data;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	if (channel->block_size != blksize) {
#ifndef NO_IO_CACHE
		if ((retval = flush_cached_blocks(channel, data, 0)))
			return retval;
#endif

		channel->block_size = blksize;
		free_cache(data);
		if ((retval = alloc_cache(channel, data)))
			return retval;
	}
	return 0;
}


static errcode_t unix_read_blk64(io_channel channel, unsigned long long block,
			       int count, void *buf)
{
	struct unix_private_data *data;
	struct unix_cache *cache, *reuse[READ_DIRECT_SIZE];
	errcode_t	retval;
	char		*cp;
	int		i, j;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct unix_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

#ifdef NO_IO_CACHE
	return raw_read_blk(channel, data, block, count, buf);
#else
	/*
	 * If we're doing an odd-sized read or a very large read,
	 * flush out the cache and then do a direct read.
	 */
	if (count < 0 || count > WRITE_DIRECT_SIZE) {
		if ((retval = flush_cached_blocks(channel, data, 0)))
			return retval;
		return raw_read_blk(channel, data, block, count, buf);
	}

	cp = buf;
	while (count > 0) {
		/* If it's in the cache, use it! */
		if ((cache = find_cached_block(data, block, &reuse[0]))) {
#ifdef DEBUG
			printf("Using cached block %lu\n", block);
#endif
			memcpy(cp, cache->buf, channel->block_size);
			count--;
			block++;
			cp += channel->block_size;
			continue;
		}
		if (count == 1) {
			/*
			 * Special case where we read directly into the
			 * cache buffer; important in the O_DIRECT case
			 */
			cache = reuse[0];
			reuse_cache(channel, data, cache, block);
			if ((retval = raw_read_blk(channel, data, block, 1,
						   cache->buf))) {
				cache->in_use = 0;
				return retval;
			}
			memcpy(cp, cache->buf, channel->block_size);
			return 0;
		}

		/*
		 * Find the number of uncached blocks so we can do a
		 * single read request
		 */
		for (i=1; i < count; i++)
			if (find_cached_block(data, block+i, &reuse[i]))
				break;
#ifdef DEBUG
		printf("Reading %d blocks starting at %lu\n", i, block);
#endif
		if ((retval = raw_read_blk(channel, data, block, i, cp)))
			return retval;

		/* Save the results in the cache */
		for (j=0; j < i; j++) {
			count--;
			cache = reuse[j];
			reuse_cache(channel, data, cache, block++);
			memcpy(cache->buf, cp, channel->block_size);
			cp += channel->block_size;
		}
	}
	return 0;
#endif /* NO_IO_CACHE */
}

static errcode_t unix_read_blk(io_channel channel, unsigned long block,
			       int count, void *buf)
{
	return unix_read_blk64(channel, block, count, buf);
}


#if defined(__linux__) && !defined(BLKDISCARD)
#define BLKDISCARD		_IO(0x12,119)
#endif

/*
io_manager.c
*/

errcode_t io_channel_read_blk64(io_channel channel, unsigned long long block,
				 int count, void *data)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	if (channel->manager->read_blk64)
		return (channel->manager->read_blk64)(channel, block,
						      count, data);

	if ((block >> 32) != 0)
		return EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	return (channel->manager->read_blk)(channel, (unsigned long) block,
					     count, data);
}

errcode_t io_channel_alloc_buf(io_channel io, int count, void *ptr)
{
	size_t	size;

	if (count == 0)
		size = io->block_size;
	else if (count > 0)
		size = io->block_size * count;
	else
		size = -count;

	if (io->align)
		return ext2fs_get_memalign(size, io->align, ptr);
	else
		return ext2fs_get_mem(size, ptr);
}


typedef unsigned int crc16_t;

/** CRC table for the CRC-16. The poly is 0x8005 (x16 + x15 + x2 + 1) */
static __u16 const crc16_table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

/**
 * Compute the CRC-16 for the data buffer
 *
 * @param crc     previous CRC value
 * @param buffer  data pointer
 * @param len     number of bytes in the buffer
 * @return        the updated CRC value
 */
crc16_t ext2fs_crc16(crc16_t crc, const void *buffer, unsigned int len)
{
	const unsigned char *cp = buffer;

	while (len--)
		/*
		 * for an unknown reason, PPC treats __u16 as signed
		 * and keeps doing sign extension on the value.
		 * Instead, use only the low 16 bits of an unsigned
		 * int for holding the CRC value to avoid this.
		 */
		crc = (((crc >> 8) & 0xffU) ^
		       crc16_table[(crc ^ *cp++) & 0xffU]) & 0x0000ffffU;
	return crc;
}

static inline int ext2fs_has_group_desc_csum(ext2_filsys fs)
{
	return ext2fs_has_feature_metadata_csum(fs->super) ||
	       ext2fs_has_feature_gdt_csum(fs->super);
}
__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	struct ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef WORDS_BIGENDIAN
	struct ext4_group_desc swabdesc;
	size_t save_size = size;
	const size_t ext4_bg_size = sizeof(struct ext4_group_desc);
	struct ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	if (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (struct ext2_group_desc *) &swabdesc);
	desc = (struct ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	if (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned char *)&group,
					 sizeof(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned char *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef WORDS_BIGENDIAN
		if (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned char *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		goto out;
	}

	/* old crc16 code */
	offset = offsetof(struct ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   sizeof(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, sizeof(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += sizeof(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	if (offset < size) {
		crc = ext2fs_crc16(crc, (char *)desc + offset,
				   size - offset);
	}
#ifdef WORDS_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	if (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (char *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	return crc;
}

int ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	if (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		return 0;

	return 1;
}

/*
 *closefs.c
 */

static int test_root(unsigned int a, unsigned int b)
{
	while (1) {
		if (a < b)
			return 0;
		if (a == b)
			return 1;
		if (a % b)
			return 0;
		a = a / b;
	}
}

int ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group)
{
	if (group == 0)
		return 1;
	if (ext2fs_has_feature_sparse_super2(fs->super)) {
		if (group == fs->super->s_backup_bgs[0] ||
		    group == fs->super->s_backup_bgs[1])
			return 1;
		return 0;
	}
	if ((group <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		return 1;
	if (!(group & 1))
		return 0;
	if (test_root(group, 3) || (test_root(group, 5)) ||
	    test_root(group, 7))
		return 1;

	return 0;
}

/*
 * ext2fs_super_and_bgd_loc2()
 * @fs:			ext2 fs pointer
 * @group		given block group
 * @ret_super_blk:	if !NULL, returns super block location
 * @ret_old_desc_blk:	if !NULL, returns location of the old block
 *			group descriptor
 * @ret_new_desc_blk:	if !NULL, returns location of meta_bg block
 *			group descriptor
 * @ret_used_blks:	if !NULL, returns number of blocks used by
 *			super block and group_descriptors.
 *
 * Returns errcode_t of 0
 */
errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
					   dgrp_t group,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_used_blks)
{
	blk64_t	group_block, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	unsigned int meta_bg, meta_bg_size;
	blk_t	numblocks = 0;
	blk64_t old_desc_blocks;
	int	has_super;

	group_block = ext2fs_group_first_block2(fs, group);
	if (group_block == 0 && fs->blocksize == 1024)
		group_block = 1; /* Deal with 1024 blocksize && bigalloc */

	if (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	else
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	has_super = ext2fs_bg_has_super(fs, group);

	if (has_super) {
		super_blk = group_block;
		numblocks++;
	}
	meta_bg_size = EXT2_DESC_PER_BLOCK(fs->super);
	meta_bg = group / meta_bg_size;

	if (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_first_meta_bg)) {
		if (has_super) {
			old_desc_blk = group_block + 1;
			numblocks += old_desc_blocks;
		}
	} else {
		if (((group % meta_bg_size) == 0) ||
		    ((group % meta_bg_size) == 1) ||
		    ((group % meta_bg_size) == (meta_bg_size-1))) {
			if (has_super)
				has_super = 1;
			new_desc_blk = group_block + has_super;
			numblocks++;
		}
	}

	if (ret_super_blk)
		*ret_super_blk = super_blk;
	if (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	if (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	if (ret_used_blks)
		*ret_used_blks = numblocks;

	return 0;
}

/*
 * This function returns the location of the superblock, block group
 * descriptors for a given block group.  It currently returns the
 * number of free blocks assuming that inode table and allocation
 * bitmaps will be in the group.  This is not necessarily the case
 * when the flex_bg feature is enabled, so callers should take care!
 * It was only really intended for use by mke2fs, and even there it's
 * not that useful.
 *
 * The ext2fs_super_and_bgd_loc2() function is 64-bit block number
 * capable and returns the number of blocks used by super block and
 * group descriptors.
 */
int ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t group,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     int *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_used_blks;
	blk_t numblocks;
	unsigned int meta_bg_size;

	ext2fs_super_and_bgd_loc2(fs, group, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_used_blks);

	numblocks = ext2fs_group_blocks_count(fs, group);

	if (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	if (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	if (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	if (ret_meta_bg) {
		meta_bg_size = EXT2_DESC_PER_BLOCK(fs->super);
		*ret_meta_bg = group / meta_bg_size;
	}

	numblocks -= 2 + fs->inode_blocks_per_group + ret_used_blks;

	return numblocks;
}

void ext2fs_init_csum_seed(ext2_filsys fs)
{
	if (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	else if (ext2fs_has_feature_metadata_csum(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 sizeof(fs->super->s_uuid));
}

static errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       struct ext2_inode_large *inode,
			       __u32 *crc, int has_hi)
{
	__u32 gen;
	struct ext2_inode_large *desc = inode;
	size_t size = fs->super->s_inode_size;
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	if (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned char *)&inum,
				sizeof(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned char *)&gen, sizeof(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned char *)desc, size);

	inode->i_checksum_lo = old_lo;
	if (has_hi)
		inode->i_checksum_hi = old_hi;
	return 0;
}

int ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     struct ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned int i, has_hi;
	char *cp;

	if (fs->super->s_creator_os != EXT2_OS_LINUX ||
	    !ext2fs_has_feature_metadata_csum(fs->super))
		return 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	if (retval)
		return 0;
	if (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} else
		calculated &= 0xFFFF;

	if (provided == calculated)
		return 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	for (cp = (char *) inode, i = 0;
	     i < sizeof(struct ext2_inode);
	     cp++, i++)
		if (*cp)
			return 0;
	return 1;		/* Inode must have been all zero's */
}

void ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	if (!ext2fs_has_group_desc_csum(fs))
		return;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

#ifndef O_DIRECT
#define O_DIRECT 0
#endif

#pragma GCC diagnostic push
#ifndef CONFIG_MMP
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

extern errcode_t ext2fs_open(const char *name, int flags, int superblock,
			     unsigned int block_size, io_manager manager,
			     ext2_filsys *ret_fs);
extern errcode_t ext2fs_open2(const char *name, const char *io_options,
			      int flags, int superblock,
			      unsigned int block_size, io_manager manager,
			      ext2_filsys *ret_fs);
errcode_t ext2fs_close(ext2_filsys fs);
errcode_t ext2fs_close2(ext2_filsys fs, int flags);
errcode_t ext2fs_close_free(ext2_filsys *fs_ptr);

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs);
errcode_t ext2fs_read_block_bitmap(ext2_filsys fs);


#define NO_SIZE            0x0010
#define VERBOSE_CREATE     0x0008
#define CREATE_FILE        0x0002
#define CHECK_FS_EXIST     0x0004

#define DBG(m,x)


#define EXT2FS_MARK_ERROR 	0
#define EXT2FS_UNMARK_ERROR 	1
#define EXT2FS_TEST_ERROR	2

static void warn_bitmap(ext2fs_generic_bitmap bitmap,
			int code, __u64 arg)
{
#ifndef OMIT_COM_ERR
	if (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", arg, bitmap->description);
	else
		com_err(0, bitmap->base_error_code + code, "#%llu", arg);
#endif
}

void ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			const char *description)
{
#ifndef OMIT_COM_ERR
	if (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	else
		com_err(0, errcode, "#%lu", arg);
#endif
}

int ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
			     __u64 arg)
{
	if (!bitmap)
		return 0;

	if (!EXT2FS_IS_64_BITMAP(bitmap))
		return 0;

	arg >>= bitmap->cluster_bits;

#ifdef ENABLE_BMAP_STATS_OPS
	if (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	if (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_count++;
#endif

	if ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		return 0;
	}

	return bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	return ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}


void ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap bmap,
				     blk64_t block, unsigned int num)
{
	__u64	end = block + num;

	if (!bmap)
		return;

	if (!EXT2FS_IS_64_BITMAP(bmap))
		return;

	INC_STAT(bmap, mark_ext_count);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1 << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	num = end - block;

	if ((block < bmap->start) || (block+num-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		return;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, num);
}

void ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap bmap,
				       blk64_t block, unsigned int num)
{
	__u64	end = block + num;

	if (!bmap)
		return;

	if (!EXT2FS_IS_64_BITMAP(bmap))
		return;

	INC_STAT(bmap, unmark_ext_count);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1 << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	num = end - block;

	if ((block < bmap->start) || (block+num-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		return;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, num);
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group.  It currently returns the number of free
 * blocks assuming that inode table and allocation bitmaps will be in
 * the group.  This is not necessarily the case when the flex_bg
 * feature is enabled, so callers should take care!  It was only
 * really intended for use by mke2fs, and even there it's not that
 * useful.  In the future, when we redo this function for 64-bit block
 * numbers, we should probably return the number of blocks used by the
 * super block and group descriptors instead.
 *
 * See also the comment for ext2fs_super_and_bgd_loc()
 */
int ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t group,
				 ext2fs_block_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	used_blks;
	int	old_desc_blocks, num_blocks;

	ext2fs_super_and_bgd_loc2(fs, group, &super_blk,
				  &old_desc_blk, &new_desc_blk, &used_blks);

	if (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	else
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	if (super_blk || (group == 0))
		ext2fs_mark_block_bitmap2(bmap, super_blk);
	if ((group == 0) && (fs->blocksize == 1024) &&
	    EXT2FS_CLUSTER_RATIO(fs) > 1)
		ext2fs_mark_block_bitmap2(bmap, 0);

	if (old_desc_blk) {
		num_blocks = old_desc_blocks;
		if (old_desc_blk + num_blocks >= ext2fs_blocks_count(fs->super))
			num_blocks = ext2fs_blocks_count(fs->super) -
				old_desc_blk;
		ext2fs_mark_block_bitmap_range2(bmap, old_desc_blk, num_blocks);
	}
	if (new_desc_blk)
		ext2fs_mark_block_bitmap2(bmap, new_desc_blk);

	num_blocks = ext2fs_group_blocks_count(fs, group);
	num_blocks -= 2 + fs->inode_blocks_per_group + used_blks;

	return num_blocks  ;
}

static errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	for (i = 0; i < fs->group_desc_count; i++) {
		if (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			continue;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		if (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		if (blk)
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		if (blk)
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	return 0;
}

int ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    char *bitmap, int size)
{
	struct ext4_group_desc *gdp = (struct ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	if (!ext2fs_has_feature_metadata_csum(fs->super))
		return 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned char *)bitmap,
				      size);
	if (fs->super->s_desc_size >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	else
		calculated &= 0xFFFF;

	return provided == calculated;
}

static errcode_t read_bitmaps(ext2_filsys fs, int do_inode, int do_block)
{
	dgrp_t i;
	char *block_bitmap = 0, *inode_bitmap = 0;
	char *buf;
	errcode_t retval;
	int block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	int inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	int csum_flag;
	unsigned int	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	if ((block_nbytes > (int) fs->blocksize) ||
	    (inode_nbytes > (int) fs->blocksize))
		return EXT2_ET_CORRUPT_SUPERBLOCK;

	//fs->write_bitmaps = ext2fs_write_bitmaps;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	if (retval)
		return retval;
	if (do_block) {
		if (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		if (retval)
			goto cleanup;
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		if (retval)
			goto cleanup;
	} else
		block_nbytes = 0;
	if (do_inode) {
		if (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		if (retval)
			goto cleanup;
		
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		if (retval)
			goto cleanup;
	} else
		inode_nbytes = 0;
	ext2fs_free_mem(&buf);

	for (i = 0; i < fs->group_desc_count; i++) {
		if (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			if (csum_flag &&
			    ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			    ext2fs_group_desc_csum_verify(fs, i))
				blk = 0;
			if (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				if (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					goto cleanup;
				}
				/* verify block bitmap checksum */
				if (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
			} else
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			if (retval)
				goto cleanup;
			blk_itr += block_nbytes << 3;
		}
		if (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			if (csum_flag &&
			    ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			    ext2fs_group_desc_csum_verify(fs, i))
				blk = 0;
			if (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				if (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					goto cleanup;
				}

				/* verify inode bitmap checksum */
				if (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
			} else
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			if (retval)
				goto cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

	/* Mark group blocks for any BLOCK_UNINIT groups */
	if (do_block) {
		retval = mark_uninit_bg_group_blocks(fs);
		if (retval)
			goto cleanup;
	}

success_cleanup:
	if (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	if (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	return 0;

cleanup:
	if (do_block) {
		ext2fs_free_mem(&fs->block_map);
		fs->block_map = 0;
	}
	if (do_inode) {
		ext2fs_free_mem(&fs->inode_map);
		fs->inode_map = 0;
	}
	if (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	if (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	if (buf)
		ext2fs_free_mem(&buf);
	return retval;
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	return read_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	return read_bitmaps(fs, 0, 1);
}

blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs, blk64_t group_block,
				     dgrp_t i)
{
	int	bg;
	int	has_super = 0, group_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	if (i == 0 && fs->blocksize == 1024 && EXT2FS_CLUSTER_RATIO(fs) > 1)
		group_zero_adjust = 1;

	if (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		return group_block + i + 1 + group_zero_adjust;

	bg = EXT2_DESC_PER_BLOCK(fs->super) * i;
	if (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_group_first_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	if (group_block != fs->super->s_first_data_block &&
	    ((ret_blk + has_super + fs->super->s_blocks_per_group) <
	     ext2fs_blocks_count(fs->super))) {
		ret_blk += fs->super->s_blocks_per_group;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		if (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		else
			has_super = 0;
	}
	return ret_blk + has_super + group_zero_adjust;
}

blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block, dgrp_t i)
{
	return ext2fs_descriptor_block_loc2(fs, group_block, i);
}

/*
 *  Allocate memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_get_mem(unsigned long size, void *ptr)
{
	void *pp;

	pp = malloc(size);
	if (!pp)
		return EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, sizeof (pp));
	return 0;
}

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, void *ptr)
{
	errcode_t retval = 0;
	void **p = ptr;

	if (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	if (retval == ENOMEM)
		return EXT2_ET_NO_MEMORY;
#else  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	if (*p == NULL) {
		if (errno)
			return errno;
		else
			return EXT2_ET_NO_MEMORY;
	}
#else  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	if (align > sizeof(long long))
		*p = valloc(size);
	else
#endif
		*p = malloc(size);
	if ((unsigned long) *p & (align - 1)) {
		free(*p);
		*p = 0;
	}
	if (*p == 0)
		return EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	return retval;
}

_INLINE_ errcode_t ext2fs_get_memzero(unsigned long size, void *ptr)
{
	void *pp;

	pp = malloc(size);
	if (!pp)
		return EXT2_ET_NO_MEMORY;
	memset(pp, 0, size);
	memcpy(ptr, &pp, sizeof(pp));
	return 0;
}

_INLINE_ errcode_t ext2fs_get_array(unsigned long count, unsigned long size, void *ptr)
{
	if (count && (~0UL)/count < size)
		return EXT2_ET_NO_MEMORY;
	return ext2fs_get_mem(count*size, ptr);
}

_INLINE_ errcode_t ext2fs_get_arrayzero(unsigned long count,
					unsigned long size, void *ptr)
{
	void *pp;

	if (count && (~0UL)/count < size)
		return EXT2_ET_NO_MEMORY;
	pp = calloc(count, size);
	if (!pp)
		return EXT2_ET_NO_MEMORY;
	memcpy(ptr, &pp, sizeof(pp));
	return 0;
}

/*
 * Free memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_free_mem(void *ptr)
{
	void *p;

	memcpy(&p, ptr, sizeof(p));
	free(p);
	p = 0;
	memcpy(ptr, &p, sizeof(p));
	return 0;
}

/*
 *  Resize memory.  The 'ptr' arg must point to a pointer.
 */
_INLINE_ errcode_t ext2fs_resize_mem(unsigned long EXT2FS_ATTR((unused)) old_size,
				     unsigned long size, void *ptr)
{
	void *p;

	/* Use "memcpy" for pointer assignments here to avoid problems
	 * with C99 strict type aliasing rules. */
	memcpy(&p, ptr, sizeof(p));
	p = realloc(p, size);
	if (!p)
		return EXT2_ET_NO_MEMORY;
	memcpy(ptr, &p, sizeof(p));
	return 0;
}

/*
 * Return the group # of a block
 */
_INLINE_ dgrp_t ext2fs_group_of_blk(ext2_filsys fs, blk_t blk)
{
	return ext2fs_group_of_blk2(fs, blk);
}
/*
 * Return the group # of an inode number
 */
_INLINE_ dgrp_t ext2fs_group_of_ino(ext2_filsys fs, ext2_ino_t ino)
{
	return (ino - 1) / fs->super->s_inodes_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_first_block(ext2_filsys fs, dgrp_t group)
{
	return (blk_t) ext2fs_group_first_block2(fs, group);
}

/*
 * Return the last block (inclusive) in a group
 */
_INLINE_ blk_t ext2fs_group_last_block(ext2_filsys fs, dgrp_t group)
{
	return (blk_t) ext2fs_group_last_block2(fs, group);
}

_INLINE_ blk_t ext2fs_inode_data_blocks(ext2_filsys fs,
					struct ext2_inode *inode)
{
	return (blk_t) ext2fs_inode_data_blocks2(fs, inode);
}

/*
 * This is an efficient, overflow safe way of calculating ceil((1.0 * a) / b)
 */
_INLINE_ unsigned int ext2fs_div_ceil(unsigned int a, unsigned int b)
{
	if (!a)
		return 0;
	return ((a - 1) / b) + 1;
}

_INLINE_ __u64 ext2fs_div64_ceil(__u64 a, __u64 b)
{
	if (!a)
		return 0;
	return ((a - 1) / b) + 1;
}


/*
 * ext3_inode has i_block array (total 60 bytes)
 * first 4 bytes are used to store:
 *  - tree depth (0 mean there is no tree yet. all extents in the inode)
 *  - number of alive extents in the inode
 */

/*
 * This is extent tail on-disk structure.
 * All other extent structures are 12 bytes long.  It turns out that
 * block_size % 12 >= 4 for at least all powers of 2 greater than 512, which
 * covers all valid ext4 block sizes.  Therefore, this tail structure can be
 * crammed into the end of the block without having to rebalance the tree.
 */


static __u16 do_nothing16(__u16 x)
{
	return x;
}

static __u16 disk_to_host16(__u16 x)
{
	return ext2fs_le16_to_cpu(x);
}

/*
 * This procedure frees a badblocks list.
 */
void ext2fs_u32_list_free(ext2_u32_list bb)
{
	if (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		return;

	if (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

void ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}

/*
 * Free a directory block list
 */
void ext2fs_free_dblist(ext2_dblist dblist)
{
	if (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		return;

	if (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	if (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

void ext2fs_free(ext2_filsys fs)
{
	if (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		return;
	if (fs->image_io != fs->io) {
		if (fs->image_io)
			io_channel_close(fs->image_io);
	}
	if (fs->io) {
		io_channel_close(fs->io);
	}
	if (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	if (fs->super)
		ext2fs_free_mem(&fs->super);
	if (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	if (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	if (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	if (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);

	if (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	if (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	if (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	if (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	if (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	fs->magic = 0;

	ext2fs_free_mem(&fs);
}

#define EXT2_CRC32C_CHKSUM		1

int ext2fs_verify_csum_type(ext2_filsys fs, struct ext2_super_block *sb)
{
	if (!ext2fs_has_feature_metadata_csum(fs->super))
		return 1;

	return sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

static __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    struct ext2_super_block *sb)
{
	int offset = offsetof(struct ext2_super_block, s_checksum);

	return ext2fs_crc32c_le(~0, (unsigned char *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
int ext2fs_superblock_csum_verify(ext2_filsys fs, struct ext2_super_block *sb)
{
	__u32 flag, calculated;

	if (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	else
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	if (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		return 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	return ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

errcode_t ext2fs_open(const char *name, int flags, int superblock,
		      unsigned int block_size, io_manager manager,
		      ext2_filsys *ret_fs)
{
	return ext2fs_open2(name, 0, flags, superblock, block_size,
			    manager, ret_fs);
}

/*
 *  Note: if superblock is non-zero, block-size must also be non-zero.
 * 	Superblock and block_size can be zero to use the default size.
 *
 * Valid flags for ext2fs_open()
 *
 * 	EXT2_FLAG_RW	- Open the filesystem for read/write.
 * 	EXT2_FLAG_FORCE - Open the filesystem even if some of the
 *				features aren't supported.
 *	EXT2_FLAG_JOURNAL_DEV_OK - Open an ext3 journal device
 *	EXT2_FLAG_SKIP_MMP - Open without multi-mount protection check.
 *	EXT2_FLAG_64BITS - Allow 64-bit bitfields (needed for large
 *				filesystems)
 */
errcode_t ext2fs_open2(const char *name, const char *io_options,
		       int flags, int superblock,
		       unsigned int block_size, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	unsigned long	i, first_meta_bg;
	__u32		features;
	unsigned int	blocks_per_group, io_flags;
	blk64_t		group_block, blk;
	char		*dest, *cp;
	int		group_zero_adjust = 0;
#ifdef WORDS_BIGENDIAN
	unsigned int	groups_per_block;
	struct ext2_group_desc *gdp;
	int		j;
#endif

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(sizeof(struct struct_ext2_filsys), &fs);
	if (retval)
		return retval;

	memset(fs, 0, sizeof(struct struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	if (retval)
		goto cleanup;
	strcpy(fs->device_name, name);

	io_flags = 0;
	if (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	if (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	if (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	if (retval)
		goto cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_SIZE, &fs->super);
	if (retval)
		goto cleanup;

	/*
	 * If the user specifies a specific block # for the
	 * superblock, then he/she must also specify the block size!
	 * Otherwise, read the master superblock located at offset
	 * SUPERBLOCK_OFFSET from the start of the partition.
	 *
	 * Note: we only save a backup copy of the superblock if we
	 * are reading the superblock from the primary superblock location.
	 */
	if (superblock) {
		if (!block_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			goto cleanup;
		}
		io_channel_set_blksize(fs->io, block_size);
		group_block = superblock;
		fs->orig_super = 0;
	} else {
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		superblock = 1;
		group_block = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
		if (retval)
			goto cleanup;
	}
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_SIZE,
				     fs->super);
	if (retval)
		goto cleanup;
	if (fs->orig_super)
		memcpy(fs->orig_super, fs->super, SUPERBLOCK_SIZE);

	if (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		if (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		if (!ext2fs_superblock_csum_verify(fs, fs->super))
			retval = EXT2_ET_SB_CSUM_INVALID;
	}

#ifdef WORDS_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
	ext2fs_swap_super(fs->super);
#else
	if (fs->flags & EXT2_FLAG_SWAP_BYTES) {
		retval = EXT2_ET_UNIMPLEMENTED;
		goto cleanup;
	}
#endif

	if (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	if (retval)
		goto cleanup;

	if (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_HIGH;
		goto cleanup;
	}

	if ((fs->super->s_log_block_size + EXT2_MIN_BLOCK_LOG_SIZE) >
	    EXT2_MAX_BLOCK_LOG_SIZE) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	if (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64BITS)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		goto cleanup;
	}

	if (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_block_size != fs->super->s_log_cluster_size)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_BLOCK_SIZE(fs->super);
	if (EXT2_INODE_SIZE(fs->super) < EXT2_GOOD_OLD_INODE_SIZE) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}

	/* Enforce the block group descriptor size */
	if (ext2fs_has_feature_64bit(fs->super)) {
		if (fs->super->s_desc_size < EXT2_MIN_DESC_SIZE_64BIT) {
			retval = EXT2_ET_BAD_DESC_SIZE;
			goto cleanup;
		}
	} else {
		if (fs->super->s_desc_size &&
		    fs->super->s_desc_size != EXT2_MIN_DESC_SIZE) {
			retval = EXT2_ET_BAD_DESC_SIZE;
			goto cleanup;
		}
	}

	fs->cluster_ratio_bits = fs->super->s_log_cluster_size -
		fs->super->s_log_block_size;
	if (EXT2_BLOCKS_PER_GROUP(fs->super) !=
	    EXT2_CLUSTERS_PER_GROUP(fs->super) << fs->cluster_ratio_bits) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}
	fs->inode_blocks_per_group = ((EXT2_INODES_PER_GROUP(fs->super) *
				       EXT2_INODE_SIZE(fs->super) +
				       EXT2_BLOCK_SIZE(fs->super) - 1) /
				      EXT2_BLOCK_SIZE(fs->super));
	if (block_size) {
		if (block_size != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_BLOCK_SIZE;
			goto cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_set_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	if (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->group_desc_count = 0;
		*ret_fs = fs;
		return 0;
	}

	if (EXT2_INODES_PER_GROUP(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_seed(fs);

	/*
	 * Read group descriptors
	 */
	blocks_per_group = EXT2_BLOCKS_PER_GROUP(fs->super);
	if (blocks_per_group == 0 ||
	    blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(fs->super) ||
	    fs->inode_blocks_per_group > EXT2_MAX_INODES_PER_GROUP(fs->super) ||
           EXT2_DESC_PER_BLOCK(fs->super) == 0 ||
           fs->super->s_first_data_block >= ext2fs_blocks_count(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}
	fs->group_desc_count = ext2fs_div64_ceil(ext2fs_blocks_count(fs->super) -
						 fs->super->s_first_data_block,
						 blocks_per_group);
	if (fs->group_desc_count * EXT2_INODES_PER_GROUP(fs->super) !=
	    fs->super->s_inodes_count) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		goto cleanup;
	}
	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_count,
					  EXT2_DESC_PER_BLOCK(fs->super));
	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	if (retval)
		goto cleanup;
	if (!group_block)
		group_block = fs->super->s_first_data_block;
	/*
	 * On a FS with a 1K blocksize, block 0 is reserved for bootloaders
	 * so we must increment block numbers to any group 0 items.
	 *
	 * However, we cannot touch group_block directly because in the meta_bg
	 * case, the ext2fs_descriptor_block_loc2() function will interpret
	 * group_block != s_first_data_block to mean that we want to access the
	 * backup group descriptors.  This is not what we want if the caller
	 * set superblock == 0 (i.e. auto-detect the superblock), which is
	 * what's going on here.
	 */
	if (group_block == 0 && fs->blocksize == 1024)
		group_zero_adjust = 1;
	dest = (char *) fs->group_desc;
#ifdef WORDS_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
#endif
	if (ext2fs_has_feature_meta_bg(fs->super)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		if (first_meta_bg > fs->desc_blocks)
			first_meta_bg = fs->desc_blocks;
	} else
		first_meta_bg = fs->desc_blocks;
	if (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, group_block +
					     group_zero_adjust + 1,
					     first_meta_bg, dest);
		if (retval)
			goto cleanup;
#ifdef WORDS_BIGENDIAN
		gdp = (struct ext2_group_desc *) dest;
		for (j=0; j < groups_per_block*first_meta_bg; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc, j);
			ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize*first_meta_bg;
	}
	for (i=first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		if (retval)
			goto cleanup;
#ifdef WORDS_BIGENDIAN
		for (j=0; j < groups_per_block; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc,
						i * groups_per_block + j);
			ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	if (superblock > 1 && ext2fs_has_group_desc_csum(fs)) {
		dgrp_t group;

		for (group = 0; group < fs->group_desc_count; group++) {
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, group, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_group_desc_csum_set(fs, group);
		}
	}

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	return 0;
cleanup:
	if (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = NULL;
	}
	*ret_fs = fs;
	return retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	if (ret)
		ext2fs_free(fs);
	*fs_ptr = NULL;
	return ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	return ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, int flags)
{
	errcode_t	retval;
	int		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	if (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		if (retval)
			return retval;
	}
	if (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	if (stats && stats->bytes_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->bytes_written >> 10;
		meta_blks = fs->desc_blocks + 1;
		if (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		if ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}

	ext2fs_free(fs);
	return 0;
}

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	if (!fs->icache)
		return 0;

	for (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	return 0;
}

/*
 * Free the inode cache structure
 */
void ext2fs_free_inode_cache(struct ext2_inode_cache *icache)
{
	unsigned i;

	if (--icache->refcount)
		return;
	if (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	for (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	if (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned int cache_size)
{
	unsigned	i;
	errcode_t	retval;

	if (fs->icache)
		return 0;
	retval = ext2fs_get_mem(sizeof(struct ext2_inode_cache), &fs->icache);
	if (retval)
		return retval;

	memset(fs->icache, 0, sizeof(struct ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	if (retval)
		goto errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  sizeof(struct ext2_inode_cache_ent),
				  &fs->icache->cache);
	if (retval)
		goto errout;

	for (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		if (retval)
			goto errout;
	}

	ext2fs_flush_icache(fs);
	return 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	return retval;
}


/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 struct ext2_inode * inode, int bufsize)
{
	blk64_t		block_nr;
	unsigned long 	group, block, offset;
	char 		*ptr;
	errcode_t	retval;
	unsigned	i;
	int		clen, inodes_per_block;
	io_channel	io;
	int		length = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large	*iptr;
	int		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/* Check to see if user has an override function */
	if (fs->read_inode &&
	    ((bufsize == sizeof(struct ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == sizeof(struct ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		if (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			return retval;
	}
	if ((ino == 0) || (ino > fs->super->s_inodes_count))
		return EXT2_ET_BAD_INODE_NUM;
	/* Create inode cache if not present */
	if (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		if (retval)
			return retval;
	}
	/* Check to see if it's in the inode cache */
	for (i = 0; i < fs->icache->cache_size; i++) {
		if (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			return 0;
		}
	}
	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	if (group > fs->group_desc_count)
		return EXT2_ET_BAD_INODE_NUM;
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	if (!ext2fs_inode_table_loc(fs, (unsigned) group))
		return EXT2_ET_MISSING_INODE_TABLE;
	block_nr = ext2fs_inode_table_loc(fs, group) +
		block;
	io = fs->io;
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (struct ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (char *) iptr;
	while (length) {
		clen = length;
		if ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		if (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			if (retval)
				return retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((char *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_inode_full(fs, (struct ext2_inode_large *) iptr,
			       (struct ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	if (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	if (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) && fail_csum)
		return EXT2_ET_INODE_CSUM_INVALID;

	return 0;
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    struct ext2_inode * inode)
{
	return ext2fs_read_inode_full(fs, ino, inode,
					sizeof(struct ext2_inode));
}

/*
 * This function resets the libc getopt() function, which keeps
 * internal state.  Bad design!  Stupid libc API designers!  No
 * biscuit!
 *
 * BSD-derived getopt() functions require that optind be reset to 1 in
 * order to reset getopt() state.  This used to be generally accepted
 * way of resetting getopt().  However, glibc's getopt()
 * has additional getopt() state beyond optind, and requires that
 * optind be set zero to reset its state.  So the unfortunate state of
 * affairs is that BSD-derived versions of getopt() misbehave if
 * optind is set to 0 in order to reset getopt(), and glibc's getopt()
 * will core dump if optind is set 1 in order to reset getopt().
 *
 * More modern versions of BSD require that optreset be set to 1 in
 * order to reset getopt().   Sigh.  Standards, anyone?
 *
 * We hide the hair here.
 */
void reset_getopt(void)
{
#if defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#else
	optind = 1;
#endif
#ifdef HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#endif
}

/*
 * this is extent on-disk structure
 * it's used at the bottom of the tree
 */
struct ext3_extent {
	__le32	ee_block;	/* first logical block extent covers */
	__le16	ee_len;		/* number of blocks covered by extent */
	__le16	ee_start_hi;	/* high 16 bits of physical block */
	__le32	ee_start;	/* low 32 bigs of physical block */
};

/*
 * this is index on-disk structure
 * it's used at all the levels, but the bottom
 */
struct ext3_extent_idx {
	__le32	ei_block;	/* index covers logical blocks from 'block' */
	__le32	ei_leaf;	/* pointer to the physical block of the next *
				 * level. leaf or next index could bet here */
	__le16	ei_leaf_hi;	/* high 16 bits of physical block */
	__le16	ei_unused;
};

/*
 * each block (leaves and indexes), even inode-stored has header
 */
struct ext3_extent_header {
	__le16	eh_magic;	/* probably will support different formats */
	__le16	eh_entries;	/* number of valid entries */
	__le16	eh_max;		/* capacity of store in entries */
	__le16	eh_depth;	/* has tree real underlaying blocks? */
	__le32	eh_generation;	/* generation of the tree */
};

#define EXT3_EXT_MAGIC		0xf30a

int main(int argc, char* argv[])
{
	struct ext2_super_block	*sb;
	struct ext2_inode inode;
	struct ext3_extent_header* eh;
	struct ext3_extent* extent;
	struct ext3_extent_idx* idx;
	
	errcode_t		retval;
	time_t			tm;
	char			buf[80];
	blk64_t			blk;
	dgrp_t			i, group, block, block_nr, offset;

	int ino;

	memset(&inode, 0, sizeof(struct ext2_inode));

	if(argc < 2){
		printf("usage: inodes dev [ino]\n");
		return 0;
	}
	retval = ext2fs_open2(argv[1], 0, EXT2_FLAG_64BITS, 0, 0,
			      unix_io_manager, &current_fs);
	if (retval)
		return;
	sb = current_fs->super;
	current_fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;

	retval = ext2fs_read_inode_bitmap(current_fs);
	if (retval) {
		com_err(argv[1], retval, "while reading inode bitmap");
		return;
	}
	retval = ext2fs_read_block_bitmap(current_fs);
	if (retval) {
		com_err(argv[1], retval, "while reading block bitmap");
		return;
	}
	printf("super block information:\n");
	printf("s_inodes_count = #%u\n", sb->s_inodes_count);
	printf("s_blocks_count = #%u\n", sb->s_blocks_count);
	printf("s_free_blocks_count = #%u\n", sb->s_free_blocks_count);
	printf("s_free_inodes_count = #%u\n", sb->s_free_inodes_count);
	printf("s_first_data_block = #%u\n", sb->s_first_data_block);
	printf("s_blocks_per_group = #%u\n", sb->s_blocks_per_group);
	printf("s_inodes_per_group = #%u\n", sb->s_inodes_per_group);
	printf("s_clusters_per_group = #%u\n", sb->s_clusters_per_group);
	printf("s_log_block_size = #%u\n", sb->s_log_block_size);
	printf("s_inode_size = #%u\n", sb->s_inode_size);
	printf("s_first_ino = #%u\n", sb->s_first_ino);
	printf("\n===============================\n");
	printf("filesys information:\n");
	printf("group_desc_count = #%u\n", current_fs->group_desc_count);
	printf("inode_map->start = #%u\n", current_fs->inode_map->start);
	printf("inode_map->end = #%u\n", current_fs->inode_map->end);
	printf("block_map->start = #%u\n", current_fs->block_map->start);
	printf("block_map->end = #%u\n", current_fs->block_map->end);
	printf("fs_blocksize = #%u\n", current_fs->blocksize);
	printf("inode_blocks_per_group = #%u\n", current_fs->inode_blocks_per_group);
	printf("s_log_groups_per_flex = #%u\n", current_fs->super->s_log_groups_per_flex);	
	printf("s_first_meta_bg = #%u\n", current_fs->super->s_first_meta_bg);	
	printf("ext2fs_has_feature_64bit=%d\n", ext2fs_has_feature_64bit(current_fs->super));
	

	/*for (i = 0; i < fs->group_desc_count; i++) {
		blk = ext2fs_inode_table_loc(fs, i);	
		printf("group %u: inode_block %u\n", i, blk);
		struct ext4_group_desc *gdp;
    	gdp = ext4fs_group_desc(fs, fs->group_desc, i);
		printf("group %u:\n\t block_bm_lo= %u block_bm_hi=%u\n", i, gdp->bg_block_bitmap, gdp->bg_block_bitmap_hi); 
		printf("\t inode_bm_li=%u inode_bm_hi=%u\n", gdp->bg_inode_bitmap, gdp->bg_inode_bitmap_hi);  
		printf("\t inode_table_li=%u inode_table_hi=%u\n", gdp->bg_inode_table, gdp->bg_inode_table_hi); 
	}*/

	if (sb->s_mtime) {
		tm = sb->s_mtime;
		if (sb->s_last_mounted[0]) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, sb->s_last_mounted,
				sizeof(sb->s_last_mounted));
			printf(_("last mounted on %s on %s"), buf,
			       ctime(&tm));
		} else
			printf(_("last mounted on %s"), ctime(&tm));
	} else if (sb->s_mkfs_time) {
		tm = sb->s_mkfs_time;
		printf(_("created on %s"), ctime(&tm));
	} else if (sb->s_wtime) {
		tm = sb->s_wtime;
		printf(_("last modified on %s"), ctime(&tm));
	}


	if(argc == 3){
		strtoblk("inodes", argv[2], 0, &ino);
		group = (ino - 1) / EXT2_INODES_PER_GROUP(current_fs->super);
		if (group > current_fs->group_desc_count)
			return EXT2_ET_BAD_INODE_NUM;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(current_fs->super)) *
			EXT2_INODE_SIZE(current_fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(current_fs->super);
		if (!ext2fs_inode_table_loc(current_fs, (unsigned) group))
			return EXT2_ET_MISSING_INODE_TABLE;
		block_nr = ext2fs_inode_table_loc(current_fs, group) + block;
		offset &= (EXT2_BLOCK_SIZE(current_fs->super) - 1);
		printf("inode %u: group %u: block %u offset %u\n", ino, group, block_nr, offset);
		retval = ext2fs_read_inode(current_fs, ino, &inode);
		if(retval)
			return retval;
		printf("inode flags 0x%x\n", inode.i_flags);
		printf("filesize (bytes unit) low=%u, high=%u\n", inode.i_size, inode.i_size_high);
		printf("fileblocks (%s unit) %u\n", (inode.i_flags & EXT4_HUGE_FILE_FL)? "4k":"512bytes", inode.i_blocks);
		if(inode.i_flags & EXT4_EXTENTS_FL){ 
			printf("\next4 using extents\n");
			printf("===============================\n");
			eh = &inode.i_block[0];
			printf("extent header: magic 0x%X, eh_entries #%u, eh_max #%u, eh_depth #%u, eh_generation #%u\n", eh->eh_magic, eh->eh_entries, eh->eh_max, eh->eh_depth, eh->eh_generation);
			char* p = (char*)(eh + 1);
			if(eh->eh_depth == 0){
				for(i = 0; i < eh->eh_entries; i++){
					extent = (struct ext3_extent_header*)(p + i * sizeof(struct ext3_extent));
					printf("extents[%u]: bn#%u, logcial bl#%u, start_hi#%u, start_lo#%u\n", i, extent->ee_len, extent->ee_block, extent->ee_start_hi, extent->ee_start);
				}
			}
			else{
				for(i = 0; i < eh->eh_entries; i++){
					idx = (struct ext3_extent_idx*)(p + i * sizeof(struct ext3_extent_idx));
					printf("extent_idx[%u]: bn#%u, leaf_hi#%u, leaf_lo#%u\n", i, idx->ei_block, idx->ei_leaf_hi, idx->ei_leaf);
				}
			}
			printf("\n");
		}
	}
	ext2fs_close_free(&current_fs);
}
