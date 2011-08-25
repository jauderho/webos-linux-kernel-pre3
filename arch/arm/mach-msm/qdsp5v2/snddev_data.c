/* Copyright (c) 2009-2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
#include <linux/platform_device.h>
#include <linux/debugfs.h>
#include <linux/mfd/msm-adie-codec.h>
#include <linux/uaccess.h>
#include <mach/qdsp5v2/snddev_icodec.h>
#include <mach/qdsp5v2/marimba_profile.h>
#include <mach/qdsp5v2/aux_pcm.h>
#include <mach/qdsp5v2/snddev_ecodec.h>
#include <mach/qdsp5v2/audio_dev_ctl.h>
#include <mach/qdsp5v2/snddev_virtual.h>
#include <mach/board.h>
#include <asm/mach-types.h>
#include <mach/gpio.h>
#include <mach/qdsp5v2/snddev_mi2s.h>
#include <mach/qdsp5v2/mi2s.h>
#include <mach/qdsp5v2/audio_acdb_def.h>

/* define the value for BT_SCO */
#define BT_SCO_PCM_CTL_VAL (PCM_CTL__RPCM_WIDTH__LINEAR_V |\
				PCM_CTL__TPCM_WIDTH__LINEAR_V)
#define BT_SCO_DATA_FORMAT_PADDING (DATA_FORMAT_PADDING_INFO__RPCM_FORMAT_V |\
				DATA_FORMAT_PADDING_INFO__TPCM_FORMAT_V)
#define BT_SCO_AUX_CODEC_INTF   AUX_CODEC_INTF_CTL__PCMINTF_DATA_EN_V

static struct adie_codec_action_unit iearpiece_48KHz_osr256_actions[] =
	HANDSET_RX_48000_OSR_256;

static struct adie_codec_hwsetting_entry iearpiece_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = iearpiece_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(iearpiece_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile iearpiece_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = iearpiece_settings,
	.setting_sz = ARRAY_SIZE(iearpiece_settings),
};

static struct snddev_icodec_data snddev_iearpiece_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_SPKR,
	.profile = &iearpiece_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 0,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2700,
	.max_voice_rx_vol[VOC_WB_INDEX] = 0,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2700
};

static struct platform_device msm_iearpiece_device = {
	.name = "snddev_icodec",
	.id = 0,
	.dev = { .platform_data = &snddev_iearpiece_data },
};

static struct snddev_icodec_data snddev_iearpiece_slider_closed_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_slider_closed_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_SPKR_SLIDER_CLOSED,
	.profile = &iearpiece_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 0,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2700,
	.max_voice_rx_vol[VOC_WB_INDEX] = 0,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2700
};

static struct platform_device msm_iearpiece_slider_closed_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HANDSET_SPKR_SLIDER_CLOSED,
	.dev = { .platform_data = &snddev_iearpiece_slider_closed_data },
};

static struct adie_codec_action_unit imic_8KHz_osr256_actions[] =
	HANDSET_TX_8000_OSR_256;

static struct adie_codec_action_unit imic_16KHz_osr256_actions[] =
	HANDSET_TX_16000_OSR_256;

static struct adie_codec_action_unit imic_48KHz_osr256_actions[] =
	HANDSET_TX_48000_OSR_256;

static struct adie_codec_hwsetting_entry imic_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = imic_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = imic_16KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_16KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = imic_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile imic_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = imic_settings,
	.setting_sz = ARRAY_SIZE(imic_settings),
};

static enum hsed_controller imic_pmctl_id[] = {PM_HSED_CONTROLLER_0};

static struct snddev_icodec_data snddev_imic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_MIC,
	.profile = &imic_profile,
	.channel_mode = 1,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = imic_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(imic_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_imic_device = {
	.name = "snddev_icodec",
	.id = 1,
	.dev = { .platform_data = &snddev_imic_data },
};

static struct adie_codec_action_unit ihs_stereo_rx_48KHz_osr256_actions[] =
	HEADSET_STEREO_RX_LEGACY_48000_OSR_256;

static struct adie_codec_hwsetting_entry ihs_stereo_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_stereo_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_stereo_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_stereo_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_stereo_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_stereo_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_stereo_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_stereo_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_SPKR_STEREO,
	.profile = &ihs_stereo_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1500,
	.max_voice_rx_vol[VOC_WB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1500
};

static struct platform_device msm_ihs_stereo_rx_device = {
	.name = "snddev_icodec",
	.id = 2,
	.dev = { .platform_data = &snddev_ihs_stereo_rx_data },
};

static struct adie_codec_action_unit ihs_mono_rx_48KHz_osr256_actions[] =
	HEADSET_RX_LEGACY_48000_OSR_256;

static struct adie_codec_hwsetting_entry ihs_mono_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_mono_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_mono_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_mono_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_mono_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_mono_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_mono_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_mono_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_SPKR_MONO,
	.profile = &ihs_mono_rx_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 400,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2300,
	.max_voice_rx_vol[VOC_WB_INDEX] = 400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2300,
};

static struct platform_device msm_ihs_mono_rx_device = {
	.name = "snddev_icodec",
	.id = 3,
	.dev = { .platform_data = &snddev_ihs_mono_rx_data },
};

static struct adie_codec_action_unit ihs_ffa_stereo_rx_48KHz_osr256_actions[] =
	HEADSET_STEREO_RX_CAPLESS_48000_OSR_256;

static struct adie_codec_hwsetting_entry ihs_ffa_stereo_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_ffa_stereo_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_ffa_stereo_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_ffa_stereo_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_ffa_stereo_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_ffa_stereo_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_ffa_stereo_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_stereo_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_SPKR_STEREO,
	.profile = &ihs_ffa_stereo_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.voltage_on = msm_snddev_hsed_voltage_on,
	.voltage_off = msm_snddev_hsed_voltage_off,
#endif
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400,
};

static struct platform_device msm_ihs_ffa_stereo_rx_device = {
	.name = "snddev_icodec",
	.id = 4,
	.dev = { .platform_data = &snddev_ihs_ffa_stereo_rx_data },
};

static struct adie_codec_action_unit ihs_ffa_mono_rx_48KHz_osr256_actions[] =
	HEADSET_RX_CAPLESS_48000_OSR_256;

static struct adie_codec_hwsetting_entry ihs_ffa_mono_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_ffa_mono_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_ffa_mono_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_ffa_mono_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_ffa_mono_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_ffa_mono_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_ffa_mono_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_mono_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_SPKR_MONO,
	.profile = &ihs_ffa_mono_rx_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = msm_snddev_hsed_voltage_on,
	.pamp_off = msm_snddev_hsed_voltage_off,
#endif
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400,
};

static struct platform_device msm_ihs_ffa_mono_rx_device = {
	.name = "snddev_icodec",
	.id = 5,
	.dev = { .platform_data = &snddev_ihs_ffa_mono_rx_data },
};

static struct adie_codec_action_unit ihs_mono_tx_8KHz_osr256_actions[] =
	HEADSET_MONO_TX_8000_OSR_256;

static struct adie_codec_action_unit ihs_mono_tx_16KHz_osr256_actions[] =
	HEADSET_MONO_TX_16000_OSR_256;

static struct adie_codec_action_unit ihs_mono_tx_48KHz_osr256_actions[] =
	HEADSET_MONO_TX_48000_OSR_256;

static struct adie_codec_hwsetting_entry ihs_mono_tx_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ihs_mono_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_mono_tx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = ihs_mono_tx_16KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_mono_tx_16KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_mono_tx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ihs_mono_tx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_mono_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = ihs_mono_tx_settings,
	.setting_sz = ARRAY_SIZE(ihs_mono_tx_settings),
};

static struct snddev_icodec_data snddev_ihs_mono_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "headset_mono_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_MIC,
	.profile = &ihs_mono_tx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_ihs_mono_tx_device = {
	.name = "snddev_icodec",
	.id = 6,
	.dev = { .platform_data = &snddev_ihs_mono_tx_data },
};

static struct adie_codec_action_unit ifmradio_handset_osr64_actions[] =
	FM_HANDSET_OSR_64;

static struct adie_codec_hwsetting_entry ifmradio_handset_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ifmradio_handset_osr64_actions,
		.action_sz = ARRAY_SIZE(ifmradio_handset_osr64_actions),
	}
};

static struct adie_codec_dev_profile ifmradio_handset_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ifmradio_handset_settings,
	.setting_sz = ARRAY_SIZE(ifmradio_handset_settings),
};

static struct snddev_icodec_data snddev_ifmradio_handset_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_FM),
	.name = "fmradio_handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_LP_FM_SPKR_PHONE_STEREO_RX,
	.profile = &ifmradio_handset_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.dev_vol_type = SNDDEV_DEV_VOL_DIGITAL,
};

static struct platform_device msm_ifmradio_handset_device = {
	.name = "snddev_icodec",
	.id = 7,
	.dev = { .platform_data = &snddev_ifmradio_handset_data },
};


static struct adie_codec_action_unit ispeaker_rx_48KHz_osr256_actions[] =
   SPEAKER_STEREO_RX_48000_OSR_256;

static struct adie_codec_hwsetting_entry ispeaker_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ispeaker_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispeaker_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ispeaker_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ispeaker_rx_settings,
	.setting_sz = ARRAY_SIZE(ispeaker_rx_settings),
};

static struct snddev_icodec_data snddev_ispeaker_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "speaker_mono_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MONO,
	.profile = &ispeaker_rx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = &msm_snddev_poweramp_on,
	.pamp_off = &msm_snddev_poweramp_off,
#else
	.pamp_on = NULL,
	.pamp_off = NULL,
#endif
	.max_voice_rx_vol[VOC_NB_INDEX] = 600,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 600,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2100,
};

static struct platform_device msm_ispeaker_rx_device = {
	.name = "snddev_icodec",
	.id = 8,
	.dev = { .platform_data = &snddev_ispeaker_rx_data },

};

static struct snddev_icodec_data snddev_ispeaker_slider_closed_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "speaker_mono_slider_closed_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MONO_SLIDER_CLOSED,
	.profile = &ispeaker_rx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = &msm_snddev_poweramp_on,
	.pamp_off = &msm_snddev_poweramp_off,
#else
	.pamp_on = NULL,
	.pamp_off = NULL,
#endif
	.max_voice_rx_vol[VOC_NB_INDEX] = 600,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 600,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2100,
};

static struct platform_device msm_ispeaker_slider_closed_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_SPKR_PHONE_MONO_SLIDER_CLOSED,
	.dev = { .platform_data = &snddev_ispeaker_slider_closed_rx_data },
};

static struct adie_codec_action_unit ifmradio_speaker_osr64_actions[] =
	FM_SPEAKER_OSR_64;

static struct adie_codec_hwsetting_entry ifmradio_speaker_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ifmradio_speaker_osr64_actions,
		.action_sz = ARRAY_SIZE(ifmradio_speaker_osr64_actions),
	}
};

static struct adie_codec_dev_profile ifmradio_speaker_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ifmradio_speaker_settings,
	.setting_sz = ARRAY_SIZE(ifmradio_speaker_settings),
};

static struct snddev_icodec_data snddev_ifmradio_speaker_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_FM),
	.name = "fmradio_speaker_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_LP_FM_SPKR_PHONE_STEREO_RX,
	.profile = &ifmradio_speaker_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = &msm_snddev_poweramp_on,
	.pamp_off = &msm_snddev_poweramp_off,
	.dev_vol_type = SNDDEV_DEV_VOL_DIGITAL,
};

static struct platform_device msm_ifmradio_speaker_device = {
	.name = "snddev_icodec",
	.id = 9,
	.dev = { .platform_data = &snddev_ifmradio_speaker_data },
};

static struct adie_codec_action_unit ifmradio_headset_osr64_actions[] =
	FM_HEADSET_STEREO_CLASS_D_LEGACY_OSR_64;

static struct adie_codec_hwsetting_entry ifmradio_headset_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ifmradio_headset_osr64_actions,
		.action_sz = ARRAY_SIZE(ifmradio_headset_osr64_actions),
	}
};

static struct adie_codec_dev_profile ifmradio_headset_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ifmradio_headset_settings,
	.setting_sz = ARRAY_SIZE(ifmradio_headset_settings),
};

static struct snddev_icodec_data snddev_ifmradio_headset_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_FM),
	.name = "fmradio_headset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_LP_FM_HEADSET_SPKR_STEREO_RX,
	.profile = &ifmradio_headset_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.dev_vol_type = SNDDEV_DEV_VOL_DIGITAL,
};

static struct platform_device msm_ifmradio_headset_device = {
	.name = "snddev_icodec",
	.id = 10,
	.dev = { .platform_data = &snddev_ifmradio_headset_data },
};


static struct adie_codec_action_unit ifmradio_ffa_headset_osr64_actions[] =
	FM_HEADSET_CLASS_AB_STEREO_CAPLESS_OSR_64;

static struct adie_codec_hwsetting_entry ifmradio_ffa_headset_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ifmradio_ffa_headset_osr64_actions,
		.action_sz = ARRAY_SIZE(ifmradio_ffa_headset_osr64_actions),
	}
};

static struct adie_codec_dev_profile ifmradio_ffa_headset_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ifmradio_ffa_headset_settings,
	.setting_sz = ARRAY_SIZE(ifmradio_ffa_headset_settings),
};

static struct snddev_icodec_data snddev_ifmradio_ffa_headset_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_FM),
	.name = "fmradio_headset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_LP_FM_HEADSET_SPKR_STEREO_RX,
	.profile = &ifmradio_ffa_headset_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = msm_snddev_hsed_voltage_on,
	.pamp_off = msm_snddev_hsed_voltage_off,
#endif
	.dev_vol_type = SNDDEV_DEV_VOL_DIGITAL,
};

static struct platform_device msm_ifmradio_ffa_headset_device = {
	.name = "snddev_icodec",
	.id = 11,
	.dev = { .platform_data = &snddev_ifmradio_ffa_headset_data },
};

static struct snddev_ecodec_data snddev_bt_sco_earpiece_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_rx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_SCO_SPKR,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
	.max_voice_rx_vol[VOC_NB_INDEX] = 400,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1100,
};

static struct snddev_ecodec_data snddev_bt_sco_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_tx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_SCO_MIC,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
};

struct platform_device msm_bt_sco_earpiece_device = {
	.name = "msm_snddev_ecodec",
	.id = 0,
	.dev = { .platform_data = &snddev_bt_sco_earpiece_data },
};

struct platform_device msm_bt_sco_mic_device = {
	.name = "msm_snddev_ecodec",
	.id = 1,
	.dev = { .platform_data = &snddev_bt_sco_mic_data },
};

static struct snddev_ecodec_data snddev_bt_sco_earpiece_with_dsp_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_with_dsp_rx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_SCO_SPKR_DSP,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
	.max_voice_rx_vol[VOC_NB_INDEX] = 400,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1100,
};

static struct snddev_ecodec_data snddev_bt_sco_mic_with_dsp_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_with_dsp_tx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_SCO_MIC_DSP,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
};

struct platform_device msm_bt_sco_earpiece_with_dsp_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_SCO_SPKR_DSP,
	.dev = { .platform_data = &snddev_bt_sco_earpiece_with_dsp_data },
};

struct platform_device msm_bt_sco_mic_with_dsp_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_SCO_MIC_DSP,
	.dev = { .platform_data = &snddev_bt_sco_mic_with_dsp_data },
};

static struct snddev_ecodec_data snddev_bt_carkit_spkr_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "bt_carkit_rx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_CARKIT_SPKR,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
	.max_voice_rx_vol[VOC_NB_INDEX] = 400,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1100,
};

static struct snddev_ecodec_data snddev_bt_carkit_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "bt_carkit_tx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_CARKIT_MIC,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
};

struct platform_device msm_bt_carkit_spkr_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_CARKIT_SPKR,
	.dev = { .platform_data = &snddev_bt_carkit_spkr_data },
};

struct platform_device msm_bt_carkit_mic_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_CARKIT_MIC,
	.dev = { .platform_data = &snddev_bt_carkit_mic_data },
};

static struct snddev_ecodec_data snddev_bt_carkit_spkr_dsp_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "bt_carkit_dsp_rx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_CARKIT_SPKR_DSP,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
	.max_voice_rx_vol[VOC_NB_INDEX] = 400,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1100,
	.max_voice_rx_vol[VOC_WB_INDEX] = 400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1100,
};

static struct snddev_ecodec_data snddev_bt_carkit_mic_dsp_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "bt_carkit_dsp_tx",
	.copp_id = 1,
	.acdb_id = ACDB_ID_BT_CARKIT_MIC_DSP,
	.channel_mode = 1,
	.conf_pcm_ctl_val = BT_SCO_PCM_CTL_VAL,
	.conf_aux_codec_intf = BT_SCO_AUX_CODEC_INTF,
	.conf_data_format_padding_val = BT_SCO_DATA_FORMAT_PADDING,
};

struct platform_device msm_bt_carkit_spkr_dsp_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_CARKIT_SPKR_DSP,
	.dev = { .platform_data = &snddev_bt_carkit_spkr_dsp_data },
};

struct platform_device msm_bt_carkit_mic_dsp_device = {
	.name = "msm_snddev_ecodec",
	.id = ACDB_ID_BT_CARKIT_MIC_DSP,
	.dev = { .platform_data = &snddev_bt_carkit_mic_dsp_data },
};

static struct adie_codec_action_unit idual_mic_endfire_8KHz_osr256_actions[] =
	MIC1_LEFT_LINE_IN_RIGHT_8000_OSR_256;

static struct adie_codec_hwsetting_entry idual_mic_endfire_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = idual_mic_endfire_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_endfire_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16KHz */
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = idual_mic_endfire_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_endfire_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 48KHz */
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = idual_mic_endfire_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_endfire_8KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile idual_mic_endfire_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = idual_mic_endfire_settings,
	.setting_sz = ARRAY_SIZE(idual_mic_endfire_settings),
};

static enum hsed_controller idual_mic_endfire_pmctl_id[] = {
	PM_HSED_CONTROLLER_0, PM_HSED_CONTROLLER_2
};

static struct snddev_icodec_data snddev_idual_mic_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_endfire_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_MIC_ENDFIRE,
	.profile = &idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_endfire_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_idual_mic_endfire_device = {
	.name = "snddev_icodec",
	.id = 12,
	.dev = { .platform_data = &snddev_idual_mic_endfire_data },
};

static struct snddev_icodec_data snddev_idual_mic_endfire_slider_closed_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_endfire_slider_closed_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_MIC_ENDFIRE_SLIDER_CLOSED,
	.profile = &idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_endfire_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_idual_mic_endfire_slider_closed_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HANDSET_MIC_ENDFIRE_SLIDER_CLOSED,
	.dev = { .platform_data = &snddev_idual_mic_endfire_slider_closed_data },
};

static struct snddev_icodec_data snddev_ihs_no_mic_slider_closed_tx_device_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "headset_no_mic_slider_closed_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_NO_MIC_SLIDER_CLOSED,
	.profile = &idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_endfire_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_ihs_no_mic_slider_closed_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HEADSET_NO_MIC_SLIDER_CLOSED,
	.dev = { .platform_data = &snddev_ihs_no_mic_slider_closed_tx_device_data },
};

static struct snddev_icodec_data snddev_ihs_no_mic_slider_open_tx_device_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "headset_no_mic_slider_open_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_NO_MIC_SLIDER_OPEN,
	.profile = &idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_endfire_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_ihs_no_mic_slider_open_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HEADSET_NO_MIC_SLIDER_OPEN,
	.dev = { .platform_data = &snddev_ihs_no_mic_slider_open_tx_device_data },
};

static struct adie_codec_action_unit idual_mic_bs_8KHz_osr256_actions[] =
	MIC1_LEFT_AUX_IN_RIGHT_8000_OSR_256;

static struct adie_codec_hwsetting_entry idual_mic_broadside_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = idual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_bs_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16KHz */
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = idual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_bs_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16KHz */
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = idual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idual_mic_bs_8KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile idual_mic_broadside_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = idual_mic_broadside_settings,
	.setting_sz = ARRAY_SIZE(idual_mic_broadside_settings),
};

static enum hsed_controller idual_mic_broadside_pmctl_id[] = {
	PM_HSED_CONTROLLER_0, PM_HSED_CONTROLLER_2
};

static struct snddev_icodec_data snddev_idual_mic_broadside_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_broadside_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_MIC_BROADSIDE,
	.profile = &idual_mic_broadside_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_broadside_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_broadside_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_idual_mic_broadside_device = {
	.name = "snddev_icodec",
	.id = 13,
	.dev = { .platform_data = &snddev_idual_mic_broadside_data },
};

static struct adie_codec_action_unit ispk_dual_mic_ef_8KHz_osr256_actions[] =
	SPEAKER_MIC1_LEFT_LINE_IN_RIGHT_8000_OSR_256;

static struct adie_codec_hwsetting_entry ispk_dual_mic_ef_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ispk_dual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_ef_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16Khz */
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = ispk_dual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_ef_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 48KHz */
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ispk_dual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_ef_8KHz_osr256_actions),
	},
};

static struct adie_codec_dev_profile ispk_dual_mic_ef_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = ispk_dual_mic_ef_settings,
	.setting_sz = ARRAY_SIZE(ispk_dual_mic_ef_settings),
};

static struct snddev_icodec_data snddev_spk_idual_mic_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_dual_mic_endfire_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC_ENDFIRE,
	.profile = &ispk_dual_mic_ef_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_endfire_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_spk_idual_mic_endfire_device = {
	.name = "snddev_icodec",
	.id = 14,
	.dev = { .platform_data = &snddev_spk_idual_mic_endfire_data },
};

static struct adie_codec_action_unit ispk_dual_mic_bs_8KHz_osr256_actions[] =
	SPEAKER_MIC1_LEFT_AUX_IN_RIGHT_8000_OSR_256;

static struct adie_codec_hwsetting_entry ispk_dual_mic_bs_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ispk_dual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_bs_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16Khz */
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = ispk_dual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_bs_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 48KHz */
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ispk_dual_mic_bs_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispk_dual_mic_bs_8KHz_osr256_actions),
	},
};

static struct adie_codec_dev_profile ispk_dual_mic_bs_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = ispk_dual_mic_bs_settings,
	.setting_sz = ARRAY_SIZE(ispk_dual_mic_bs_settings),
};
static struct snddev_icodec_data snddev_spk_idual_mic_broadside_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_dual_mic_broadside_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC_BROADSIDE,
	.profile = &ispk_dual_mic_bs_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = idual_mic_broadside_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(idual_mic_broadside_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_spk_idual_mic_broadside_device = {
	.name = "snddev_icodec",
	.id = 15,
	.dev = { .platform_data = &snddev_spk_idual_mic_broadside_data },
};

static struct adie_codec_action_unit itty_hs_mono_tx_8KHz_osr256_actions[] =
	TTY_HEADSET_MONO_TX_8000_OSR_256;

static struct adie_codec_hwsetting_entry itty_hs_mono_tx_settings[] = {
	/* 8KHz, 16KHz, 48KHz TTY Tx devices can shared same set of actions */
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = itty_hs_mono_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_tx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = itty_hs_mono_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_tx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = itty_hs_mono_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_tx_8KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile itty_hs_mono_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = itty_hs_mono_tx_settings,
	.setting_sz = ARRAY_SIZE(itty_hs_mono_tx_settings),
};

static struct snddev_icodec_data snddev_itty_hs_mono_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE | SNDDEV_CAP_TTY),
	.name = "tty_headset_mono_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_TTY_HEADSET_MIC,
	.profile = &itty_hs_mono_tx_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_itty_hs_mono_tx_device = {
	.name = "snddev_icodec",
	.id = 16,
	.dev = { .platform_data = &snddev_itty_hs_mono_tx_data },
};

static struct adie_codec_action_unit itty_hs_mono_rx_8KHz_osr256_actions[] =
	TTY_HEADSET_MONO_RX_CLASS_D_8000_OSR_256;

static struct adie_codec_action_unit itty_hs_mono_rx_16KHz_osr256_actions[] =
	TTY_HEADSET_MONO_RX_CLASS_D_16000_OSR_256;

static struct adie_codec_action_unit itty_hs_mono_rx_48KHz_osr256_actions[] =
	TTY_HEADSET_MONO_RX_CLASS_D_48000_OSR_256;

static struct adie_codec_hwsetting_entry itty_hs_mono_rx_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = itty_hs_mono_rx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_rx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = itty_hs_mono_rx_16KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_rx_16KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = itty_hs_mono_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(itty_hs_mono_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile itty_hs_mono_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = itty_hs_mono_rx_settings,
	.setting_sz = ARRAY_SIZE(itty_hs_mono_rx_settings),
};

static struct snddev_icodec_data snddev_itty_hs_mono_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE | SNDDEV_CAP_TTY),
	.name = "tty_headset_mono_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_TTY_HEADSET_SPKR,
	.profile = &itty_hs_mono_rx_profile,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 0,
	.min_voice_rx_vol[VOC_NB_INDEX] = 0,
	.max_voice_rx_vol[VOC_WB_INDEX] = 0,
	.min_voice_rx_vol[VOC_WB_INDEX] = 0,
};

static struct platform_device msm_itty_hs_mono_rx_device = {
	.name = "snddev_icodec",
	.id = 17,
	.dev = { .platform_data = &snddev_itty_hs_mono_rx_data },
};

static struct adie_codec_action_unit ispeaker_tx_8KHz_osr256_actions[] =
	SPEAKER_TX_8000_OSR_256;

static struct adie_codec_action_unit ispeaker_tx_48KHz_osr256_actions[] =
	SPEAKER_TX_48000_OSR_256;

static struct adie_codec_hwsetting_entry ispeaker_tx_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = ispeaker_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispeaker_tx_8KHz_osr256_actions),
	},
	{ /* 8KHz profile is good for 16KHz */
		.freq_plan = 16000,
		.osr = 256,
		.actions = ispeaker_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispeaker_tx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ispeaker_tx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispeaker_tx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ispeaker_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = ispeaker_tx_settings,
	.setting_sz = ARRAY_SIZE(ispeaker_tx_settings),
};

static enum hsed_controller ispk_pmctl_id[] = {PM_HSED_CONTROLLER_0};

static struct snddev_icodec_data snddev_ispeaker_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_stereo_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC,
	.profile = &ispeaker_tx_profile,
	.channel_mode = 2,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = ispk_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(ispk_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = msm_snddev_tx_route_config,
	.pamp_off = msm_snddev_tx_route_deconfig,
#else
	.pamp_on = NULL,
	.pamp_off = NULL,
#endif
};

static struct platform_device msm_ispeaker_tx_device = {
	.name = "snddev_icodec",
	.id = 18,
	.dev = { .platform_data = &snddev_ispeaker_tx_data },
};

static struct snddev_icodec_data snddev_ispeaker_slider_closed_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_stereo_slider_closed_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC_SLIDER_CLOSED,
	.profile = &ispeaker_tx_profile,
	.channel_mode = 2,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = ispk_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(ispk_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = msm_snddev_tx_route_config,
	.pamp_off = msm_snddev_tx_route_deconfig,
#else
	.pamp_on = NULL,
	.pamp_off = NULL,
#endif
};

static struct platform_device msm_ispeaker_slider_closed_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_SPKR_PHONE_MIC_SLIDER_CLOSED,
	.dev = { .platform_data = &snddev_ispeaker_slider_closed_tx_data },
};

static struct adie_codec_action_unit iearpiece_ffa_48KHz_osr256_actions[] =
	HANDSET_RX_48000_OSR_256_FFA;

static struct adie_codec_hwsetting_entry iearpiece_ffa_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = iearpiece_ffa_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(iearpiece_ffa_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile iearpiece_ffa_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = iearpiece_ffa_settings,
	.setting_sz = ARRAY_SIZE(iearpiece_ffa_settings),
};

static struct snddev_icodec_data snddev_iearpiece_ffa_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_SPKR,
	.profile = &iearpiece_ffa_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -1400,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2900,
};

static struct platform_device msm_iearpiece_ffa_device = {
	.name = "snddev_icodec",
	.id = 19,
	.dev = { .platform_data = &snddev_iearpiece_ffa_data },
};

static struct adie_codec_action_unit imic_ffa_8KHz_osr256_actions[] =
	HANDSET_TX_8000_OSR_256_FFA;

static struct adie_codec_action_unit imic_ffa_16KHz_osr256_actions[] =
	HANDSET_TX_16000_OSR_256_FFA;

static struct adie_codec_action_unit imic_ffa_48KHz_osr256_actions[] =
	HANDSET_TX_48000_OSR_256_FFA;

static struct adie_codec_hwsetting_entry imic_ffa_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = imic_ffa_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_ffa_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = imic_ffa_16KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_ffa_16KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = imic_ffa_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_ffa_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile imic_ffa_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = imic_ffa_settings,
	.setting_sz = ARRAY_SIZE(imic_ffa_settings),
};

static struct snddev_icodec_data snddev_imic_ffa_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HANDSET_MIC,
	.profile = &imic_ffa_profile,
	.channel_mode = 1,
#ifdef CONFIG_MARIMBA_CODEC
	.pmctl_id = imic_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(imic_pmctl_id),
#else
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
#endif
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_imic_ffa_device = {
	.name = "snddev_icodec",
	.id = 20,
	.dev = { .platform_data = &snddev_imic_ffa_data },
};


static struct adie_codec_action_unit
	ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions[] =
	HEADSET_STEREO_SPEAKER_STEREO_RX_CAPLESS_48000_OSR_256;


static struct adie_codec_hwsetting_entry
	ihs_stereo_speaker_stereo_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions,
		.action_sz =
		ARRAY_SIZE(ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_stereo_speaker_stereo_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_stereo_speaker_stereo_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_stereo_speaker_stereo_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_stereo_speaker_stereo_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_stereo_speaker_stereo_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HEADSET_STEREO_PLUS_SPKR_STEREO_RX,
	.profile = &ihs_stereo_speaker_stereo_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
#ifdef CONFIG_MARIMBA_CODEC
	.pamp_on = msm_snddev_poweramp_on,
	.pamp_off = msm_snddev_poweramp_off,
	.voltage_on = msm_snddev_hsed_voltage_on,
	.voltage_off = msm_snddev_hsed_voltage_off,
#endif
	.max_voice_rx_vol[VOC_NB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1500,
	.max_voice_rx_vol[VOC_WB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1500,
};

static struct platform_device msm_ihs_stereo_speaker_stereo_rx_device = {
	.name = "snddev_icodec",
	.id = 21,
	.dev = { .platform_data = &snddev_ihs_stereo_speaker_stereo_rx_data },
};

static struct snddev_mi2s_data snddev_mi2s_stereo_rx_data = {
	.capability = SNDDEV_CAP_RX ,
	.name = "hdmi_stereo_rx",
	.copp_id = 3,
	.acdb_id = ACDB_ID_HDMI,
	.channel_mode = 2,
	.sd_lines = MI2S_SD_0,
	.route = msm_snddev_tx_route_config,
	.deroute = msm_snddev_tx_route_deconfig,
	.default_sample_rate = 48000,
};

static struct platform_device msm_snddev_mi2s_stereo_rx_device = {
	.name = "snddev_mi2s",
	.id = 0,
	.dev = { .platform_data = &snddev_mi2s_stereo_rx_data },
};


static struct snddev_mi2s_data snddev_mi2s_fm_tx_data = {
	.capability = SNDDEV_CAP_TX ,
	.name = "fmradio_stereo_tx",
	.copp_id = 2,
	.acdb_id = ACDB_ID_FM_TX,
	.channel_mode = 2,
	.sd_lines = MI2S_SD_3,
	.route = NULL,
	.deroute = NULL,
	.default_sample_rate = 48000,
};

static struct platform_device  msm_snddev_mi2s_fm_tx_device = {
	.name = "snddev_mi2s",
	.id = 1,
	.dev = { .platform_data = &snddev_mi2s_fm_tx_data},
};

static struct snddev_icodec_data snddev_fluid_imic_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC,
	.profile = &ispeaker_tx_profile,
	.channel_mode = 1,
	.pmctl_id = ispk_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(ispk_pmctl_id),
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_tx_route_config,
	.pamp_off = msm_snddev_tx_route_deconfig,
};

static struct platform_device msm_fluid_imic_tx_device = {
	.name = "snddev_icodec",
	.id = 22,
	.dev = { .platform_data = &snddev_fluid_imic_tx_data },
};

static struct snddev_icodec_data snddev_fluid_iearpiece_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_STEREO,
	.profile = &ispeaker_rx_profile,
	.channel_mode = 2,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 48000,
	.pamp_on = &msm_snddev_poweramp_on,
	.pamp_off = &msm_snddev_poweramp_off,
	.max_voice_rx_vol[VOC_NB_INDEX] = -500,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1000,
	.max_voice_rx_vol[VOC_WB_INDEX] = -500,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1000,
};

static struct platform_device msm_fluid_iearpeice_rx_device = {
	.name = "snddev_icodec",
	.id = 23,
	.dev = { .platform_data = &snddev_fluid_iearpiece_rx_data },
};

static struct adie_codec_action_unit fluid_idual_mic_ef_8KHz_osr256_actions[] =
	MIC1_LEFT_AUX_IN_RIGHT_8000_OSR_256;

static struct adie_codec_hwsetting_entry fluid_idual_mic_endfire_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = fluid_idual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(fluid_idual_mic_ef_8KHz_osr256_actions),
	}, /* 8KHz profile can be used for 16KHz */
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = fluid_idual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(fluid_idual_mic_ef_8KHz_osr256_actions),
	}, /* 8KHz profile can also be used for 48KHz */
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = fluid_idual_mic_ef_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(fluid_idual_mic_ef_8KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile fluid_idual_mic_endfire_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = fluid_idual_mic_endfire_settings,
	.setting_sz = ARRAY_SIZE(fluid_idual_mic_endfire_settings),
};

static enum hsed_controller fluid_idual_mic_endfire_pmctl_id[] = {
	PM_HSED_CONTROLLER_0, PM_HSED_CONTROLLER_2
};

static struct snddev_icodec_data snddev_fluid_idual_mic_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_endfire_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC_ENDFIRE,
	.profile = &fluid_idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pmctl_id = fluid_idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(fluid_idual_mic_endfire_pmctl_id),
	.pamp_on = msm_snddev_tx_route_config,
	.pamp_off = msm_snddev_tx_route_deconfig,
};

static struct platform_device msm_fluid_idual_mic_endfire_device = {
	.name = "snddev_icodec",
	.id = 24,
	.dev = { .platform_data = &snddev_fluid_idual_mic_endfire_data },
};

static struct snddev_icodec_data snddev_fluid_spk_idual_mic_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_dual_mic_endfire_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_SPKR_PHONE_MIC_ENDFIRE,
	.profile = &fluid_idual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pmctl_id = fluid_idual_mic_endfire_pmctl_id,
	.pmctl_id_sz = ARRAY_SIZE(fluid_idual_mic_endfire_pmctl_id),
	.pamp_on = msm_snddev_tx_route_config,
	.pamp_off = msm_snddev_tx_route_deconfig,
};

static struct platform_device msm_fluid_spk_idual_mic_endfire_device = {
	.name = "snddev_icodec",
	.id = 25,
	.dev = { .platform_data = &snddev_fluid_spk_idual_mic_endfire_data },
};

static struct snddev_virtual_data snddev_a2dp_tx_data = {
	.capability = SNDDEV_CAP_TX,
	.name = "a2dp_tx",
	.copp_id = 5,
	.acdb_id = PSEUDO_ACDB_ID,
};

static struct snddev_virtual_data snddev_a2dp_rx_data = {
	.capability = SNDDEV_CAP_RX,
	.name = "a2dp_rx",
	.copp_id = 2,
	.acdb_id = PSEUDO_ACDB_ID,
};

static struct platform_device msm_a2dp_rx_device = {
	.name = "snddev_virtual",
	.id = 0,
	.dev = { .platform_data = &snddev_a2dp_rx_data },
};

static struct platform_device msm_a2dp_tx_device = {
	.name = "snddev_virtual",
	.id = 1,
	.dev = { .platform_data = &snddev_a2dp_tx_data },
};

static struct snddev_virtual_data snddev_uplink_rx_data = {
	.capability = SNDDEV_CAP_RX,
	.name = "uplink_rx",
	.copp_id = 5,
	.acdb_id = PSEUDO_ACDB_ID,
};

static struct platform_device msm_uplink_rx_device = {
	.name = "snddev_virtual",
	.id = 2,
	.dev = { .platform_data = &snddev_uplink_rx_data },
};






/* Added audeqoff tx and rx device for factory testing to disable all audio processing on modem*/
static struct adie_codec_action_unit audeqoff_rx_48KHz_osr256_actions[] =
	HEADSET_STEREO_RX_LEGACY_48000_OSR_256;

static struct adie_codec_hwsetting_entry audeqoff_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = audeqoff_rx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(audeqoff_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile audeqoff_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = audeqoff_rx_settings,
	.setting_sz = ARRAY_SIZE(audeqoff_rx_settings),
};

static struct snddev_icodec_data snddev_audeqoff_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "audeqoff_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_AUDEQOFF_RX,
	.profile = &audeqoff_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_NB_INDEX] = -1500,
	.max_voice_rx_vol[VOC_WB_INDEX] = 1200,
	.min_voice_rx_vol[VOC_WB_INDEX] = -1500,
};

static struct platform_device msm_audeqoff_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_AUDEQOFF_RX,
	.dev = { .platform_data = &snddev_audeqoff_rx_data },
};


static struct adie_codec_action_unit audeqoff_tx_8KHz_osr256_actions[] =
	HEADSET_MONO_TX_8000_OSR_256;

static struct adie_codec_action_unit audeqoff_tx_16KHz_osr256_actions[] =
	HEADSET_MONO_TX_16000_OSR_256;

static struct adie_codec_action_unit audeqoff_tx_48KHz_osr256_actions[] =
	HEADSET_MONO_TX_48000_OSR_256;

static struct adie_codec_hwsetting_entry audeqoff_tx_settings[] = {
	{
		.freq_plan = 8000,
		.osr = 256,
		.actions = audeqoff_tx_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(audeqoff_tx_8KHz_osr256_actions),
	},
	{
		.freq_plan = 16000,
		.osr = 256,
		.actions = audeqoff_tx_16KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(audeqoff_tx_16KHz_osr256_actions),
	},
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = audeqoff_tx_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(audeqoff_tx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile audeqoff_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = audeqoff_tx_settings,
	.setting_sz = ARRAY_SIZE(audeqoff_tx_settings),
};

static struct snddev_icodec_data snddev_audeqoff_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "audeqoff_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_AUDEQOFF_TX,
	.profile = &audeqoff_tx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_audeqoff_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_AUDEQOFF_TX,
	.dev = { .platform_data = &snddev_audeqoff_tx_data },
};

static struct snddev_icodec_data snddev_hac_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "hac_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HAC_RX,
	.profile = NULL,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 0,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2400,
	.max_voice_rx_vol[VOC_WB_INDEX] = 0,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400,
};

static struct platform_device msm_hac_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HAC_RX,
	.dev = { .platform_data = &snddev_hac_rx_data },
};

static struct snddev_icodec_data snddev_hac_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "hac_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HAC_TX,
	.profile = NULL,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_hac_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HAC_TX,
	.dev = { .platform_data = &snddev_hac_tx_data },
};

static struct snddev_icodec_data snddev_hac_slider_closed_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "hac_slider_closed_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HAC_SLIDER_CLOSED_RX,
	.profile = NULL,
	.channel_mode = 1,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = 0,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2400,
	.max_voice_rx_vol[VOC_WB_INDEX] = 0,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400,
};

static struct platform_device msm_hac_slider_closed_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HAC_SLIDER_CLOSED_RX,
	.dev = { .platform_data = &snddev_hac_slider_closed_rx_data },
};

static struct snddev_icodec_data snddev_hac_slider_closed_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "hac_slider_closed_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_HAC_SLIDER_CLOSED_TX,
	.profile = NULL,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_hac_slider_closed_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_HAC_SLIDER_CLOSED_TX,
	.dev = { .platform_data = &snddev_hac_slider_closed_tx_data },
};

/* All the media mode audio profiles */
static struct adie_codec_hwsetting_entry media_headset_tx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_headset_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = media_headset_tx_settings,
	.setting_sz = ARRAY_SIZE(media_headset_tx_settings),
};

static struct snddev_icodec_data snddev_media_headset_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "media_headset_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_HEADSET_TX,
	.profile = &media_headset_tx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_media_headset_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_HEADSET_TX,
	.dev = { .platform_data = &snddev_media_headset_tx_data },
};


static struct adie_codec_hwsetting_entry media_headset_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_headset_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = media_headset_rx_settings,
	.setting_sz = ARRAY_SIZE(media_headset_rx_settings),
};

static struct snddev_icodec_data snddev_media_headset_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "media_headset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_HEADSET_RX,
	.profile = &media_headset_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_media_headset_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_HEADSET_RX,
	.dev = { .platform_data = &snddev_media_headset_rx_data },
};



static struct adie_codec_hwsetting_entry media_headset_mic_tx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_headset_mic_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = media_headset_mic_tx_settings,
	.setting_sz = ARRAY_SIZE(media_headset_mic_tx_settings),
};

static struct snddev_icodec_data snddev_media_headset_mic_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "media_headset_mic_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_HEADSET_MIC_TX,
	.profile = &media_headset_mic_tx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_media_headset_mic_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_HEADSET_MIC_TX,
	.dev = { .platform_data = &snddev_media_headset_mic_tx_data },
};


static struct adie_codec_hwsetting_entry media_headset_mic_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_headset_mic_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = media_headset_mic_rx_settings,
	.setting_sz = ARRAY_SIZE(media_headset_mic_rx_settings),
};

static struct snddev_icodec_data snddev_media_headset_mic_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "media_headset_mic_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_HEADSET_MIC_RX,
	.profile = &media_headset_mic_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_media_headset_mic_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_HEADSET_MIC_RX,
	.dev = { .platform_data = &snddev_media_headset_mic_rx_data },
};



static struct adie_codec_hwsetting_entry media_back_speaker_tx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_back_speaker_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = media_back_speaker_tx_settings,
	.setting_sz = ARRAY_SIZE(media_back_speaker_tx_settings),
};

static struct snddev_icodec_data snddev_media_back_speaker_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "media_back_speaker_tx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_BACK_SPEAKER_TX,
	.profile = &media_back_speaker_tx_profile,
	.channel_mode = 1,
	.pmctl_id = NULL,
	.pmctl_id_sz = 0,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
};

static struct platform_device msm_media_back_speaker_tx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_BACK_SPEAKER_TX,
	.dev = { .platform_data = &snddev_media_back_speaker_tx_data },
};


static struct adie_codec_hwsetting_entry media_back_speaker_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_back_speaker_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = media_back_speaker_rx_settings,
	.setting_sz = ARRAY_SIZE(media_back_speaker_rx_settings),
};

static struct snddev_icodec_data snddev_media_back_speaker_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "media_back_speaker_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_BACK_SPEAKER_RX,
	.profile = &media_back_speaker_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_media_back_speaker_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_BACK_SPEAKER_RX,
	.dev = { .platform_data = &snddev_media_back_speaker_rx_data },
};



static struct adie_codec_hwsetting_entry media_handset_tx_settings[] = {
    {
        .freq_plan = 48000,
        .osr = 256,
        .actions = NULL,
        .action_sz = 0,
    }
};

static struct adie_codec_dev_profile media_handset_tx_profile = {
    .path_type = ADIE_CODEC_TX,
    .settings = media_handset_tx_settings,
    .setting_sz = ARRAY_SIZE(media_handset_tx_settings),
};

static struct snddev_icodec_data snddev_media_handset_tx_data = {
    .capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
    .name = "media_handset_tx",
    .copp_id = 0,
    .acdb_id = ACDB_ID_MEDIA_HANDSET_TX,
    .profile = &media_handset_tx_profile,
    .channel_mode = 1,
    .pmctl_id = NULL,
    .pmctl_id_sz = 0,
    .default_sample_rate = 48000,
    .pamp_on = NULL,
    .pamp_off = NULL,
};

static struct platform_device msm_media_handset_tx_device = {
    .name = "snddev_icodec",
    .id = ACDB_ID_MEDIA_HANDSET_TX,
    .dev = { .platform_data = &snddev_media_handset_tx_data },
};


static struct adie_codec_hwsetting_entry media_handset_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile media_handset_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = media_handset_rx_settings,
	.setting_sz = ARRAY_SIZE(media_handset_rx_settings),
};

static struct snddev_icodec_data snddev_media_handset_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "media_handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_MEDIA_HANDSET_RX,
	.profile = &media_handset_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_media_handset_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_MEDIA_HANDSET_RX,
	.dev = { .platform_data = &snddev_media_handset_rx_data },
};



static struct adie_codec_hwsetting_entry vc_headset_tx_settings[] = {
    {
        .freq_plan = 48000,
        .osr = 256,
        .actions = NULL,
        .action_sz = 0,
    }
};

static struct adie_codec_dev_profile vc_headset_tx_profile = {
    .path_type = ADIE_CODEC_TX,
    .settings = vc_headset_tx_settings,
    .setting_sz = ARRAY_SIZE(vc_headset_tx_settings),
};

static struct snddev_icodec_data snddev_vc_headset_tx_data = {
    .capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
    .name = "vc_headset_tx",
    .copp_id = 0,
    .acdb_id = ACDB_ID_VC_HEADSET_TX,
    .profile = &vc_headset_tx_profile,
    .channel_mode = 1,
    .pmctl_id = NULL,
    .pmctl_id_sz = 0,
    .default_sample_rate = 8000,
    .pamp_on = NULL,
    .pamp_off = NULL,
};

static struct platform_device msm_vc_headset_tx_device = {
    .name = "snddev_icodec",
    .id = ACDB_ID_VC_HEADSET_TX,
    .dev = { .platform_data = &snddev_vc_headset_tx_data },
};


static struct adie_codec_hwsetting_entry vc_headset_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile vc_headset_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = vc_headset_rx_settings,
	.setting_sz = ARRAY_SIZE(vc_headset_rx_settings),
};

static struct snddev_icodec_data snddev_vc_headset_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "vc_headset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_VC_HEADSET_RX,
	.profile = &vc_headset_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_vc_headset_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_VC_HEADSET_RX,
	.dev = { .platform_data = &snddev_vc_headset_rx_data },
};



static struct adie_codec_hwsetting_entry vc_headset_mic_tx_settings[] = {
    {
        .freq_plan = 48000,
        .osr = 256,
        .actions = NULL,
        .action_sz = 0,
    }
};

static struct adie_codec_dev_profile vc_headset_mic_tx_profile = {
    .path_type = ADIE_CODEC_TX,
    .settings = vc_headset_mic_tx_settings,
    .setting_sz = ARRAY_SIZE(vc_headset_mic_tx_settings),
};

static struct snddev_icodec_data snddev_vc_headset_mic_tx_data = {
    .capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
    .name = "vc_headset_mic_tx",
    .copp_id = 0,
    .acdb_id = ACDB_ID_VC_HEADSET_MIC_TX,
    .profile = &vc_headset_mic_tx_profile,
    .channel_mode = 1,
    .pmctl_id = NULL,
    .pmctl_id_sz = 0,
    .default_sample_rate = 8000,
    .pamp_on = NULL,
    .pamp_off = NULL,
};

static struct platform_device msm_vc_headset_mic_tx_device = {
    .name = "snddev_icodec",
    .id = ACDB_ID_VC_HEADSET_MIC_TX,
    .dev = { .platform_data = &snddev_vc_headset_mic_tx_data },
};


static struct adie_codec_hwsetting_entry vc_headset_mic_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile vc_headset_mic_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = vc_headset_mic_rx_settings,
	.setting_sz = ARRAY_SIZE(vc_headset_mic_rx_settings),
};

static struct snddev_icodec_data snddev_vc_headset_mic_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "vc_headset_mic_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_VC_HEADSET_MIC_RX,
	.profile = &vc_headset_mic_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_vc_headset_mic_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_VC_HEADSET_MIC_RX,
	.dev = { .platform_data = &snddev_vc_headset_mic_rx_data },
};



static struct adie_codec_hwsetting_entry vc_back_speaker_tx_settings[] = {
    {
        .freq_plan = 48000,
        .osr = 256,
        .actions = NULL,
        .action_sz = 0,
    }
};

static struct adie_codec_dev_profile vc_back_speaker_tx_profile = {
    .path_type = ADIE_CODEC_TX,
    .settings = vc_back_speaker_tx_settings,
    .setting_sz = ARRAY_SIZE(vc_back_speaker_tx_settings),
};

static struct snddev_icodec_data snddev_vc_back_speaker_tx_data = {
    .capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
    .name = "vc_back_speaker_tx",
    .copp_id = 0,
    .acdb_id = ACDB_ID_VC_BACK_SPEAKER_TX,
    .profile = &vc_back_speaker_tx_profile,
    .channel_mode = 1,
    .pmctl_id = NULL,
    .pmctl_id_sz = 0,
    .default_sample_rate = 8000,
    .pamp_on = NULL,
    .pamp_off = NULL,
};

static struct platform_device msm_vc_back_speaker_tx_device = {
    .name = "snddev_icodec",
    .id = ACDB_ID_VC_BACK_SPEAKER_TX,
    .dev = { .platform_data = &snddev_vc_back_speaker_tx_data },
};


static struct adie_codec_hwsetting_entry vc_back_speaker_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile vc_back_speaker_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = vc_back_speaker_rx_settings,
	.setting_sz = ARRAY_SIZE(vc_back_speaker_rx_settings),
};

static struct snddev_icodec_data snddev_vc_back_speaker_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "vc_back_speaker_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_VC_BACK_SPEAKER_RX,
	.profile = &vc_back_speaker_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_vc_back_speaker_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_VC_BACK_SPEAKER_RX,
	.dev = { .platform_data = &snddev_vc_back_speaker_rx_data },
};



static struct adie_codec_hwsetting_entry vc_handset_tx_settings[] = {
    {
        .freq_plan = 48000,
        .osr = 256,
        .actions = NULL,
        .action_sz = 0,
    }
};

static struct adie_codec_dev_profile vc_handset_tx_profile = {
    .path_type = ADIE_CODEC_TX,
    .settings = vc_handset_tx_settings,
    .setting_sz = ARRAY_SIZE(vc_handset_tx_settings),
};

static struct snddev_icodec_data snddev_vc_handset_tx_data = {
    .capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
    .name = "vc_handset_tx",
    .copp_id = 0,
    .acdb_id = ACDB_ID_VC_HANDSET_TX,
    .profile = &vc_handset_tx_profile,
    .channel_mode = 1,
    .pmctl_id = NULL,
    .pmctl_id_sz = 0,
    .default_sample_rate = 8000,
    .pamp_on = NULL,
    .pamp_off = NULL,
};

static struct platform_device msm_vc_handset_tx_device = {
    .name = "snddev_icodec",
    .id = ACDB_ID_VC_HANDSET_TX,
    .dev = { .platform_data = &snddev_vc_handset_tx_data },
};


static struct adie_codec_hwsetting_entry vc_handset_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = NULL,
		.action_sz = 0,
	}
};

static struct adie_codec_dev_profile vc_handset_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = vc_handset_rx_settings,
	.setting_sz = ARRAY_SIZE(vc_handset_rx_settings),
};

static struct snddev_icodec_data snddev_vc_handset_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "vc_handset_rx",
	.copp_id = 0,
	.acdb_id = ACDB_ID_VC_HANDSET_RX,
	.profile = &vc_handset_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 8000,
	.pamp_on = NULL,
	.pamp_off = NULL,
	.max_voice_rx_vol[VOC_NB_INDEX] = -700,
	.min_voice_rx_vol[VOC_NB_INDEX] = -2200,
	.max_voice_rx_vol[VOC_WB_INDEX] = -900,
	.min_voice_rx_vol[VOC_WB_INDEX] = -2400
};

static struct platform_device msm_vc_handset_rx_device = {
	.name = "snddev_icodec",
	.id = ACDB_ID_VC_HANDSET_RX,
	.dev = { .platform_data = &snddev_vc_handset_rx_data },
};

static struct platform_device *snd_devices_ffa[] __initdata = {
	&msm_iearpiece_ffa_device,
	&msm_imic_ffa_device,
	&msm_ifmradio_handset_device,
	&msm_ihs_ffa_stereo_rx_device,
	&msm_ihs_ffa_mono_rx_device,
	&msm_ihs_mono_tx_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_ispeaker_rx_device,
	&msm_ifmradio_speaker_device,
	&msm_ifmradio_ffa_headset_device,
	&msm_idual_mic_endfire_device,
	&msm_idual_mic_broadside_device,
	&msm_spk_idual_mic_endfire_device,
	&msm_spk_idual_mic_broadside_device,
	&msm_itty_hs_mono_tx_device,
	&msm_itty_hs_mono_rx_device,
	&msm_ispeaker_tx_device,
	&msm_ihs_stereo_speaker_stereo_rx_device,
	&msm_a2dp_rx_device,
	&msm_a2dp_tx_device,
	&msm_snddev_mi2s_stereo_rx_device,
	&msm_snddev_mi2s_fm_tx_device,
	&msm_uplink_rx_device,
};

static struct platform_device *snd_devices_surf[] __initdata = {
	&msm_iearpiece_device,
	&msm_imic_device,
	&msm_ihs_stereo_rx_device,
	&msm_ihs_mono_rx_device,
	&msm_ihs_mono_tx_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_ifmradio_handset_device,
	&msm_ispeaker_rx_device,
	&msm_ifmradio_speaker_device,
	&msm_ifmradio_headset_device,
	&msm_itty_hs_mono_tx_device,
	&msm_itty_hs_mono_rx_device,
	&msm_ispeaker_tx_device,
	&msm_ihs_stereo_speaker_stereo_rx_device,
	&msm_a2dp_rx_device,
	&msm_a2dp_tx_device,
	&msm_snddev_mi2s_stereo_rx_device,
	&msm_snddev_mi2s_fm_tx_device,
	&msm_uplink_rx_device,
};

static struct platform_device *snd_devices_fluid[] __initdata = {
	&msm_ihs_stereo_rx_device,
	&msm_ihs_mono_rx_device,
	&msm_ihs_mono_tx_device,
	&msm_ispeaker_rx_device,
	&msm_ispeaker_tx_device,
	&msm_fluid_imic_tx_device,
	&msm_fluid_iearpeice_rx_device,
	&msm_fluid_idual_mic_endfire_device,
	&msm_fluid_spk_idual_mic_endfire_device,
	&msm_a2dp_rx_device,
	&msm_a2dp_tx_device,
	&msm_snddev_mi2s_stereo_rx_device,
	&msm_uplink_rx_device,
	&msm_ifmradio_speaker_device,
	&msm_ifmradio_headset_device,
};

static struct platform_device *snd_devices_shank[] __initdata = {
	&msm_iearpiece_device,
	&msm_imic_device,
	&msm_ihs_stereo_rx_device,
	&msm_ihs_mono_tx_device,
	&msm_ispeaker_rx_device,
	&msm_ispeaker_tx_device,
	&msm_itty_hs_mono_rx_device,
	&msm_itty_hs_mono_tx_device,
	&msm_idual_mic_endfire_device,
	&msm_idual_mic_broadside_device,
	&msm_spk_idual_mic_endfire_device,
	&msm_spk_idual_mic_broadside_device,
	&msm_iearpiece_slider_closed_device,
	&msm_idual_mic_endfire_slider_closed_device,
	&msm_ispeaker_slider_closed_rx_device,
	&msm_ispeaker_slider_closed_tx_device,
	&msm_ihs_no_mic_slider_open_tx_device,
	&msm_ihs_no_mic_slider_closed_tx_device,
	&msm_media_headset_rx_device,
	&msm_media_headset_tx_device,
	&msm_media_headset_mic_rx_device,
	&msm_media_headset_mic_tx_device,
	&msm_media_back_speaker_rx_device,
	&msm_media_back_speaker_tx_device,
	&msm_media_handset_rx_device,
	&msm_media_handset_tx_device,
	&msm_vc_headset_rx_device,
	&msm_vc_headset_tx_device,
	&msm_vc_headset_mic_rx_device,
	&msm_vc_headset_mic_tx_device,
	&msm_vc_back_speaker_rx_device,
	&msm_vc_back_speaker_tx_device,
	&msm_vc_handset_rx_device,
	&msm_vc_handset_tx_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_bt_sco_earpiece_with_dsp_device,
	&msm_bt_sco_mic_with_dsp_device,
	&msm_audeqoff_rx_device,
	&msm_audeqoff_tx_device,
};

static struct platform_device *snd_devices_rib[] __initdata = {
	&msm_iearpiece_device,
	&msm_imic_device,
	&msm_ihs_mono_rx_device,
	&msm_ihs_mono_tx_device,
	&msm_ispeaker_rx_device,
	&msm_ispeaker_tx_device,
	&msm_itty_hs_mono_rx_device,
	&msm_itty_hs_mono_tx_device,
	&msm_idual_mic_endfire_device,
	&msm_idual_mic_broadside_device,
	&msm_spk_idual_mic_endfire_device,
	&msm_spk_idual_mic_broadside_device,
	&msm_iearpiece_slider_closed_device,
	&msm_idual_mic_endfire_slider_closed_device,
	&msm_ispeaker_slider_closed_rx_device,
	&msm_ispeaker_slider_closed_tx_device,
	&msm_ihs_no_mic_slider_open_tx_device,
	&msm_ihs_no_mic_slider_closed_tx_device,
	&msm_media_headset_rx_device,
	&msm_media_headset_tx_device,
	&msm_media_headset_mic_rx_device,
	&msm_media_headset_mic_tx_device,
	&msm_media_back_speaker_rx_device,
	&msm_media_back_speaker_tx_device,
	&msm_media_handset_rx_device,
	&msm_media_handset_tx_device,
	&msm_vc_headset_rx_device,
	&msm_vc_headset_tx_device,
	&msm_vc_headset_mic_rx_device,
	&msm_vc_headset_mic_tx_device,
	&msm_vc_back_speaker_rx_device,
	&msm_vc_back_speaker_tx_device,
	&msm_vc_handset_rx_device,
	&msm_vc_handset_tx_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_bt_sco_earpiece_with_dsp_device,
	&msm_bt_sco_mic_with_dsp_device,
	&msm_bt_carkit_spkr_device,
	&msm_bt_carkit_mic_device,
	&msm_bt_carkit_spkr_dsp_device,
	&msm_bt_carkit_mic_dsp_device,
	&msm_audeqoff_rx_device,
	&msm_audeqoff_tx_device,
	&msm_hac_rx_device,
	&msm_hac_tx_device,
	&msm_hac_slider_closed_rx_device,
	&msm_hac_slider_closed_tx_device,
};

void __init msm_snddev_init(void)
{
	if (machine_is_msm7x30_ffa() || machine_is_msm8x55_ffa() ||
		machine_is_msm8x55_svlte_ffa()) {
		platform_add_devices(snd_devices_ffa,
		ARRAY_SIZE(snd_devices_ffa));
	} else if (machine_is_msm7x30_surf() || machine_is_msm8x55_surf() ||
		machine_is_msm8x55_svlte_surf())
		platform_add_devices(snd_devices_surf,
		ARRAY_SIZE(snd_devices_surf));
	else if (machine_is_msm7x30_fluid())
		platform_add_devices(snd_devices_fluid,
		ARRAY_SIZE(snd_devices_fluid));
	else if (machine_is_rib())
		platform_add_devices(snd_devices_rib,
		ARRAY_SIZE(snd_devices_rib));
	else if (machine_is_shank())
		platform_add_devices(snd_devices_shank,
		ARRAY_SIZE(snd_devices_shank));
	else
		pr_err("%s: Unknown machine type\n", __func__);
}
