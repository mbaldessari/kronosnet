/*
 * Copyright (C) 2010-2012 Red Hat, Inc.  All rights reserved.
 *
 * Authors: Fabio M. Di Nitto <fabbione@kronosnet.org>
 *          Federico Simoncelli <fsimon@kronosnet.org>
 *
 * This software licensed under GPL-2.0+, LGPL-2.0+
 */

#ifndef __ONWIRE_H__
#define __ONWIRE_H__

#include <stdint.h>

#define KNET_HOST_INFO_LINK_UP_DOWN 0
#define KNET_HOST_INFO_LINK_TABLE   1

#if 0
struct knet_hinfo_link {
	uint8_t			khl_link_id;
	uint8_t			khl_link_dynamic;
	uint8_t			khl_link_priority;
	uint64_t		khl_link_latency;
	char			khl_link_dst_ipaddr[KNET_MAX_HOST_LEN];
	char			khl_link_dst_port[KNET_MAX_PORT_LEN];
} __attribute__((packed));

struct knet_hinfo_link_table {
	uint16_t		khlt_node_id;
	uint8_t			khlt_local; /* we have this node connected locally */
	struct knet_hinfo_link	khlt_link[KNET_MAX_LINK]; /* info we send about each link in the node */
} __attribute__((packed));
#endif

union knet_hinfo_dtype {
	struct {
		uint8_t		khdt_link_id;
		uint8_t		khdt_link_status;
	} link_up_down;
#if 0
	struct {
		uint16_t	khdt_host_entries;
		uint8_t		khdt_host_maps[0]; /* array of knet_hinfo_link_table[khdt_host_entries] */
	} link_table __attribute__((packed));
#endif
} __attribute__((packed));

struct knet_hinfo_data {			/* this is sent in kf_data */
	uint8_t			khd_type;	/* link_up_down / link_table */
	uint8_t			khd_bcast;	/* bcast/ucast */
	uint16_t		khd_dst_node_id;/* used only if in ucast mode */
	union knet_hinfo_dtype  khd_dype;
} __attribute__((packed));

union knet_frame_data {
	struct {
		seq_num_t	kfd_seq_num;
		uint8_t		kfd_data[0];
	} data __attribute__((packed));
	struct {
		uint8_t		kfd_link;
		uint32_t	kfd_time[4];
	} ping __attribute__((packed));
	struct {
		uint8_t		kfd_link;
		uint16_t	kfd_pmtud_size;
		uint8_t		kfd_data[0];
	} pmtud __attribute__((packed));
} __attribute__((packed));

struct knet_frame {
	uint8_t			kf_version;
	uint8_t			kf_type;
	uint16_t		kf_node;
	union knet_frame_data	kf_payload;
} __attribute__((packed));

#define kf_seq_num kf_payload.data.kfd_seq_num
#define kf_data    kf_payload.data.kfd_data
#define kf_link    kf_payload.ping.kfd_link
#define kf_time    kf_payload.ping.kfd_time
#define kf_dyn     kf_payload.ping.kfd_dyn
#define kf_plink   kf_payload.pmtud.kfd_link
#define kf_psize   kf_payload.pmtud.kfd_pmtud_size
#define kf_pdata   kf_payload.pmtud.kfd_data

#define KNET_PING_SIZE sizeof(struct knet_frame)
#define KNET_FRAME_SIZE (sizeof(struct knet_frame) - sizeof(union knet_frame_data))

/* taken from tracepath6 */
#define KNET_PMTUD_SIZE_V4 65535
#define KNET_PMTUD_SIZE_V6 128000
#define KNET_PMTUD_OVERHEAD_V4 28
#define KNET_PMTUD_OVERHEAD_V6 48
#define KNET_PMTUD_MIN_MTU_V4 576
#define KNET_PMTUD_MIN_MTU_V6 1280

#define KNET_FRAME_VERSION 0x01

#define KNET_FRAME_DATA        0x00
#define KNET_FRAME_HOST_INFO   0x01
#define KNET_FRAME_PING        0x81
#define KNET_FRAME_PONG        0x82
#define KNET_FRAME_PMTUD       0x83
#define KNET_FRAME_PMTUD_REPLY 0x84
#define KNET_FRAME_PMSK        0x80 /* ping/pong packet mask */

#endif
