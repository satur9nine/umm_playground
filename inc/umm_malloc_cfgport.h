#ifndef _UMM_MALLOC_CFG_PORT_H
#define _UMM_MALLOC_CFG_PORT_H

#define UMM_BLOCK_BODY_SIZE (16)

#ifdef __cplusplus
extern "C" {
#endif

void umm_port_critical_entry(void);
void umm_port_critical_exit(void);

#ifdef __cplusplus
}
#endif

#define UMM_CRITICAL_DECL(tag)
#define UMM_CRITICAL_ENTRY(tag) umm_port_critical_entry()
#define UMM_CRITICAL_EXIT(tag) umm_port_critical_exit()

#define UMM_INFO

#endif // _UMM_MALLOC_CFG_PORT_H
