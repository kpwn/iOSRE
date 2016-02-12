#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <sound/asound.h>
#include <sound/asound_fm.h>
#include <sound/asequencer.h>
#include <sound/hdsp.h>
#include <sound/hdspm.h>
#include <sound/sb16_csp.h>
#include <sound/sfnt_info.h>
#ifdef USE_SNDDRV_COMPRESS_OFFLOAD
#include <sound/compress_offload.h>
#endif

/* would use this, but the header uses DECLARE_BITMAP() from the kernel */
/* #include <sound/emu10k1.h> */

#include "trinity.h"
#include "ioctls.h"

/* include/sound/hda_hwdep.h */
struct hda_verb_ioctl {
        __u32 verb;       /* HDA_VERB() */
        __u32 res;        /* response */
};
#define HDA_IOCTL_PVERSION              _IOR('H', 0x10, int)
#define HDA_IOCTL_VERB_WRITE            _IOWR('H', 0x11, struct hda_verb_ioctl)
#define HDA_IOCTL_GET_WCAP              _IOWR('H', 0x12, struct hda_verb_ioctl)

static const struct ioctl sound_ioctls[] = {
	IOCTL(SNDRV_SEQ_IOCTL_PVERSION),
	IOCTL(SNDRV_SEQ_IOCTL_CLIENT_ID),
	IOCTL(SNDRV_SEQ_IOCTL_SYSTEM_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_RUNNING_MODE),
	IOCTL(SNDRV_SEQ_IOCTL_GET_CLIENT_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_SET_CLIENT_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_CREATE_PORT),
	IOCTL(SNDRV_SEQ_IOCTL_DELETE_PORT),
	IOCTL(SNDRV_SEQ_IOCTL_GET_PORT_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_SET_PORT_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_SUBSCRIBE_PORT),
	IOCTL(SNDRV_SEQ_IOCTL_UNSUBSCRIBE_PORT),
	IOCTL(SNDRV_SEQ_IOCTL_CREATE_QUEUE),
	IOCTL(SNDRV_SEQ_IOCTL_DELETE_QUEUE),
	IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_INFO),
	IOCTL(SNDRV_SEQ_IOCTL_GET_NAMED_QUEUE),
	IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_STATUS),
	IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_TEMPO),
	IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_TEMPO),
	/* IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_OWNER), */
	/* IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_OWNER), */
	IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_TIMER),
	IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_TIMER),
	/* IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_SYNC), */
	/* IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_SYNC), */
	IOCTL(SNDRV_SEQ_IOCTL_GET_QUEUE_CLIENT),
	IOCTL(SNDRV_SEQ_IOCTL_SET_QUEUE_CLIENT),
	IOCTL(SNDRV_SEQ_IOCTL_GET_CLIENT_POOL),
	IOCTL(SNDRV_SEQ_IOCTL_SET_CLIENT_POOL),
	IOCTL(SNDRV_SEQ_IOCTL_REMOVE_EVENTS),
	IOCTL(SNDRV_SEQ_IOCTL_QUERY_SUBS),
	IOCTL(SNDRV_SEQ_IOCTL_GET_SUBSCRIPTION),
	IOCTL(SNDRV_SEQ_IOCTL_QUERY_NEXT_CLIENT),
	IOCTL(SNDRV_SEQ_IOCTL_QUERY_NEXT_PORT),
	IOCTL(SNDRV_DM_FM_IOCTL_INFO),
	IOCTL(SNDRV_DM_FM_IOCTL_RESET),
	IOCTL(SNDRV_DM_FM_IOCTL_PLAY_NOTE),
	IOCTL(SNDRV_DM_FM_IOCTL_SET_VOICE),
	IOCTL(SNDRV_DM_FM_IOCTL_SET_PARAMS),
	IOCTL(SNDRV_DM_FM_IOCTL_SET_MODE),
	IOCTL(SNDRV_DM_FM_IOCTL_SET_CONNECTION),
	IOCTL(SNDRV_DM_FM_IOCTL_CLEAR_PATCHES),
	IOCTL(SNDRV_HWDEP_IOCTL_PVERSION),
	IOCTL(SNDRV_HWDEP_IOCTL_INFO),
	IOCTL(SNDRV_HWDEP_IOCTL_DSP_STATUS),
	IOCTL(SNDRV_HWDEP_IOCTL_DSP_LOAD),
	IOCTL(SNDRV_PCM_IOCTL_PVERSION),
	IOCTL(SNDRV_PCM_IOCTL_INFO),
	IOCTL(SNDRV_PCM_IOCTL_TSTAMP),
	IOCTL(SNDRV_PCM_IOCTL_TTSTAMP),
	IOCTL(SNDRV_PCM_IOCTL_HW_REFINE),
	IOCTL(SNDRV_PCM_IOCTL_HW_PARAMS),
	IOCTL(SNDRV_PCM_IOCTL_HW_FREE),
	IOCTL(SNDRV_PCM_IOCTL_SW_PARAMS),
	IOCTL(SNDRV_PCM_IOCTL_STATUS),
	IOCTL(SNDRV_PCM_IOCTL_DELAY),
	IOCTL(SNDRV_PCM_IOCTL_HWSYNC),
	IOCTL(SNDRV_PCM_IOCTL_SYNC_PTR),
	IOCTL(SNDRV_PCM_IOCTL_CHANNEL_INFO),
	IOCTL(SNDRV_PCM_IOCTL_PREPARE),
	IOCTL(SNDRV_PCM_IOCTL_RESET),
	IOCTL(SNDRV_PCM_IOCTL_START),
	IOCTL(SNDRV_PCM_IOCTL_DROP),
	IOCTL(SNDRV_PCM_IOCTL_DRAIN),
	IOCTL(SNDRV_PCM_IOCTL_PAUSE),
	IOCTL(SNDRV_PCM_IOCTL_REWIND),
	IOCTL(SNDRV_PCM_IOCTL_RESUME),
	IOCTL(SNDRV_PCM_IOCTL_XRUN),
	IOCTL(SNDRV_PCM_IOCTL_FORWARD),
	IOCTL(SNDRV_PCM_IOCTL_WRITEI_FRAMES),
	IOCTL(SNDRV_PCM_IOCTL_READI_FRAMES),
	IOCTL(SNDRV_PCM_IOCTL_WRITEN_FRAMES),
	IOCTL(SNDRV_PCM_IOCTL_READN_FRAMES),
	IOCTL(SNDRV_PCM_IOCTL_LINK),
	IOCTL(SNDRV_PCM_IOCTL_UNLINK),
	IOCTL(SNDRV_RAWMIDI_IOCTL_PVERSION),
	IOCTL(SNDRV_RAWMIDI_IOCTL_INFO),
	IOCTL(SNDRV_RAWMIDI_IOCTL_PARAMS),
	IOCTL(SNDRV_RAWMIDI_IOCTL_STATUS),
	IOCTL(SNDRV_RAWMIDI_IOCTL_DROP),
	IOCTL(SNDRV_RAWMIDI_IOCTL_DRAIN),
	IOCTL(SNDRV_TIMER_IOCTL_PVERSION),
	IOCTL(SNDRV_TIMER_IOCTL_NEXT_DEVICE),
	IOCTL(SNDRV_TIMER_IOCTL_TREAD),
	IOCTL(SNDRV_TIMER_IOCTL_GINFO),
	IOCTL(SNDRV_TIMER_IOCTL_GPARAMS),
	IOCTL(SNDRV_TIMER_IOCTL_GSTATUS),
	IOCTL(SNDRV_TIMER_IOCTL_SELECT),
	IOCTL(SNDRV_TIMER_IOCTL_INFO),
	IOCTL(SNDRV_TIMER_IOCTL_PARAMS),
	IOCTL(SNDRV_TIMER_IOCTL_STATUS),
	IOCTL(SNDRV_TIMER_IOCTL_START),
	IOCTL(SNDRV_TIMER_IOCTL_STOP),
	IOCTL(SNDRV_TIMER_IOCTL_CONTINUE),
	IOCTL(SNDRV_TIMER_IOCTL_PAUSE),
	IOCTL(SNDRV_CTL_IOCTL_PVERSION),
	IOCTL(SNDRV_CTL_IOCTL_CARD_INFO),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_LIST),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_INFO),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_READ),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_WRITE),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_LOCK),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_UNLOCK),
	IOCTL(SNDRV_CTL_IOCTL_SUBSCRIBE_EVENTS),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_ADD),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_REPLACE),
	IOCTL(SNDRV_CTL_IOCTL_ELEM_REMOVE),
	IOCTL(SNDRV_CTL_IOCTL_TLV_READ),
	IOCTL(SNDRV_CTL_IOCTL_TLV_WRITE),
	IOCTL(SNDRV_CTL_IOCTL_TLV_COMMAND),
	IOCTL(SNDRV_CTL_IOCTL_HWDEP_NEXT_DEVICE),
	IOCTL(SNDRV_CTL_IOCTL_HWDEP_INFO),
	IOCTL(SNDRV_CTL_IOCTL_PCM_NEXT_DEVICE),
	IOCTL(SNDRV_CTL_IOCTL_PCM_INFO),
	IOCTL(SNDRV_CTL_IOCTL_PCM_PREFER_SUBDEVICE),
	IOCTL(SNDRV_CTL_IOCTL_RAWMIDI_NEXT_DEVICE),
	IOCTL(SNDRV_CTL_IOCTL_RAWMIDI_INFO),
	IOCTL(SNDRV_CTL_IOCTL_RAWMIDI_PREFER_SUBDEVICE),
	IOCTL(SNDRV_CTL_IOCTL_POWER),
	IOCTL(SNDRV_CTL_IOCTL_POWER_STATE),
	IOCTL(HDA_IOCTL_PVERSION),
	IOCTL(HDA_IOCTL_VERB_WRITE),
	IOCTL(HDA_IOCTL_GET_WCAP),
	IOCTL(SNDRV_HDSP_IOCTL_GET_PEAK_RMS),
	IOCTL(SNDRV_HDSP_IOCTL_GET_CONFIG_INFO),
	IOCTL(SNDRV_HDSP_IOCTL_UPLOAD_FIRMWARE),
	IOCTL(SNDRV_HDSP_IOCTL_GET_VERSION),
	IOCTL(SNDRV_HDSP_IOCTL_GET_MIXER),
	IOCTL(SNDRV_HDSP_IOCTL_GET_9632_AEB),
	IOCTL(SNDRV_HDSPM_IOCTL_GET_VERSION),
	IOCTL(SNDRV_HDSPM_IOCTL_GET_MIXER),
	IOCTL(SNDRV_SB_CSP_IOCTL_INFO),
	IOCTL(SNDRV_SB_CSP_IOCTL_LOAD_CODE),
	IOCTL(SNDRV_SB_CSP_IOCTL_UNLOAD_CODE),
	IOCTL(SNDRV_SB_CSP_IOCTL_START),
	IOCTL(SNDRV_SB_CSP_IOCTL_STOP),
	IOCTL(SNDRV_SB_CSP_IOCTL_PAUSE),
	IOCTL(SNDRV_SB_CSP_IOCTL_RESTART),
	IOCTL(SNDRV_EMUX_IOCTL_VERSION),
	IOCTL(SNDRV_EMUX_IOCTL_LOAD_PATCH),
	IOCTL(SNDRV_EMUX_IOCTL_RESET_SAMPLES),
	IOCTL(SNDRV_EMUX_IOCTL_REMOVE_LAST_SAMPLES),
	IOCTL(SNDRV_EMUX_IOCTL_MEM_AVAIL),
	IOCTL(SNDRV_EMUX_IOCTL_MISC_MODE),

	/*
	IOCTL(SNDRV_EMU10K1_IOCTL_INFO),
	IOCTL(SNDRV_EMU10K1_IOCTL_CODE_POKE),
	IOCTL(SNDRV_EMU10K1_IOCTL_CODE_PEEK),
	IOCTL(SNDRV_EMU10K1_IOCTL_TRAM_SETUP),
	IOCTL(SNDRV_EMU10K1_IOCTL_TRAM_POKE),
	IOCTL(SNDRV_EMU10K1_IOCTL_TRAM_PEEK),
	IOCTL(SNDRV_EMU10K1_IOCTL_PCM_POKE),
	IOCTL(SNDRV_EMU10K1_IOCTL_PCM_PEEK),
	IOCTL(SNDRV_EMU10K1_IOCTL_PVERSION),
	IOCTL(SNDRV_EMU10K1_IOCTL_STOP),
	IOCTL(SNDRV_EMU10K1_IOCTL_CONTINUE),
	IOCTL(SNDRV_EMU10K1_IOCTL_ZERO_TRAM_COUNTER),
	IOCTL(SNDRV_EMU10K1_IOCTL_SINGLE_STEP),
	IOCTL(SNDRV_EMU10K1_IOCTL_DBG_READ),
	*/
	{ .name = "SNDRV_EMU10K1_IOCTL_INFO", .request = _IOC(_IOC_NONE,'H',0x10,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_CODE_POKE", .request = _IOC(_IOC_NONE,'H',0x11,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_CODE_PEEK", .request = _IOC(_IOC_NONE,'H',0x12,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_TRAM_SETUP", .request = _IOC(_IOC_NONE,'H',0x20,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_TRAM_POKE", .request = _IOC(_IOC_NONE,'H',0x21,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_TRAM_PEEK", .request = _IOC(_IOC_NONE,'H',0x22,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_PCM_POKE", .request = _IOC(_IOC_NONE,'H',0x30,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_PCM_PEEK", .request = _IOC(_IOC_NONE,'H',0x31,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_PVERSION", .request = _IOC(_IOC_NONE,'H',0x40,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_STOP", .request = _IOC(_IOC_NONE,'H',0x80,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_CONTINUE", .request = _IOC(_IOC_NONE,'H',0x81,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_ZERO_TRAM_COUNTER", .request = _IOC(_IOC_NONE,'H',0x82,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_SINGLE_STEP", .request = _IOC(_IOC_NONE,'H',0x83,0), },
	{ .name = "SNDRV_EMU10K1_IOCTL_DBG_READ", .request = _IOC(_IOC_NONE,'H',0x84,0), },

#ifdef USE_SNDDRV_COMPRESS_OFFLOAD
	IOCTL(SNDRV_COMPRESS_IOCTL_VERSION),
	IOCTL(SNDRV_COMPRESS_GET_CAPS),
	IOCTL(SNDRV_COMPRESS_GET_CODEC_CAPS),
	IOCTL(SNDRV_COMPRESS_SET_PARAMS),
	IOCTL(SNDRV_COMPRESS_GET_PARAMS),
#ifdef SNDRV_COMPRESS_SET_METADATA
	IOCTL(SNDRV_COMPRESS_SET_METADATA),
#endif
#ifdef SNDRV_COMPRESS_GET_METADATA
	IOCTL(SNDRV_COMPRESS_GET_METADATA),
#endif
	IOCTL(SNDRV_COMPRESS_TSTAMP),
	IOCTL(SNDRV_COMPRESS_AVAIL),
	IOCTL(SNDRV_COMPRESS_PAUSE),
	IOCTL(SNDRV_COMPRESS_RESUME),
	IOCTL(SNDRV_COMPRESS_START),
	IOCTL(SNDRV_COMPRESS_STOP),
	IOCTL(SNDRV_COMPRESS_DRAIN),
#ifdef SNDRV_COMPRESS_NEXT_TRACK
	IOCTL(SNDRV_COMPRESS_NEXT_TRACK),
#endif
#ifdef SNDRV_COMPRESS_PARTIAL_DRAIN
	IOCTL(SNDRV_COMPRESS_PARTIAL_DRAIN),
#endif
#endif /* USE_SNDDRV_COMPRESS_OFFLOAD */
};

static const char *const sound_devs[] = {
	"sound",
	"alsa",
};

static const struct ioctl_group sound_grp = {
	.devtype = DEV_CHAR,
	.devs = sound_devs,
	.devs_cnt = ARRAY_SIZE(sound_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = sound_ioctls,
	.ioctls_cnt = ARRAY_SIZE(sound_ioctls),
};

REG_IOCTL_GROUP(sound_grp)
