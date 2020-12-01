PROG=proj

SRCS = proj.c Videocard.c KeyBoard.c mouse.c timer.c utils.c Player.c Game.c Menus.c Buttons.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
