PROG=proj

SRCS = proj.c Videocard.c KeyBoard.c mouse.c timer.c utils.c Player.c Game.c Menus.c Buttons.c Rooms.c Projectile.c Tasks.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D__LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
