/**
 * @file      rtk_bt_att_defs.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth ATT related type definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_ATT_DEFS_H__
#define __RTK_BT_ATT_DEFS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_types.h>

/**
 * @def       RTK_BT_LE_DEFAULT_ATT_MTU_SIZE
 * @brief     Bluetooth default ATT MTU size.
 */
#define RTK_BT_LE_DEFAULT_ATT_MTU_SIZE      23

/**
 * @def       BT_UUID_SIZE_16
 * @brief     Bluetooth UUID16 size.
 */
#define BT_UUID_SIZE_16 2

/**
 * @def       BT_UUID_SIZE_32
 * @brief     Bluetooth UUID32 size.
 */
#define BT_UUID_SIZE_32 4

/**
 * @def       BT_UUID_SIZE_128
 * @brief     Bluetooth UUID128 size.
 */
#define BT_UUID_SIZE_128 16

/**
 * @def       BT_UUID_SIZE_MAX
 * @brief     Bluetooth UUIDMAX size.
 */
#define BT_UUID_SIZE_MAX BT_UUID_SIZE_128

/**
 * @def       BT_UUID_GATT_PRIMARY_VAL
 * @brief     GATT Primary Service UUID value
 */
#define BT_UUID_GATT_PRIMARY_VAL			0x2800

/**
 * @def       BT_UUID_GATT_PRIMARY
 * @brief     GATT Primary Service
 */
#define BT_UUID_GATT_PRIMARY \
	BT_UUID_DECLARE_16(BT_UUID_GATT_PRIMARY_VAL)

/**
 * @def       BT_UUID_GATT_SECONDARY_VAL
 * @brief     GATT Secondary Service UUID value
 */
#define BT_UUID_GATT_SECONDARY_VAL			0x2801

/**
 * @def       BT_UUID_GATT_SECONDARY
 * @brief     GATT Secondary Service
 */
#define BT_UUID_GATT_SECONDARY \
	BT_UUID_DECLARE_16(BT_UUID_GATT_SECONDARY_VAL)

/**
 * @def       BT_UUID_GATT_INCLUDE_VAL
 * @brief     GATT Include Service UUID value
 */
#define BT_UUID_GATT_INCLUDE_VAL			0x2802

/**
 * @def       BT_UUID_GATT_INCLUDE
 * @brief     GATT Include Service
 */
#define BT_UUID_GATT_INCLUDE \
	BT_UUID_DECLARE_16(BT_UUID_GATT_INCLUDE_VAL)

/**
 * @def       BT_UUID_GATT_CHRC_VAL
 * @brief     GATT Characteristic UUID value
 */
#define BT_UUID_GATT_CHRC_VAL				0x2803

/**
 * @def       BT_UUID_GATT_CHRC
 * @brief     GATT Characteristic
 */
#define BT_UUID_GATT_CHRC \
	BT_UUID_DECLARE_16(BT_UUID_GATT_CHRC_VAL)

/**
 * @def       BT_UUID_GATT_CEP_VAL
 * @brief     GATT Characteristic Extended Properties UUID value
 */
#define BT_UUID_GATT_CEP_VAL				0x2900

/**
 * @def       BT_UUID_GATT_CEP
 * @brief     GATT Characteristic Extended Properties
 */
#define BT_UUID_GATT_CEP \
	BT_UUID_DECLARE_16(BT_UUID_GATT_CEP_VAL)

/**
 * @def       BT_UUID_GATT_CUD_VAL
 * @brief     GATT Characteristic User Description UUID value
 */
#define BT_UUID_GATT_CUD_VAL				0x2901

/**
 * @def       BT_UUID_GATT_CUD
 * @brief     GATT Characteristic User Description
 */
#define BT_UUID_GATT_CUD \
	BT_UUID_DECLARE_16(BT_UUID_GATT_CUD_VAL)

/**
 * @def       BT_UUID_GATT_CCC_VAL
 * @brief     GATT Client Characteristic Configuration UUID value
 */
#define BT_UUID_GATT_CCC_VAL				0x2902

/**
 * @def       BT_UUID_GATT_CCC
 * @brief     GATT Client Characteristic Configuration
 */
#define BT_UUID_GATT_CCC \
	BT_UUID_DECLARE_16(BT_UUID_GATT_CCC_VAL)

/**
 * @def       BT_UUID_GATT_SCC_VAL
 * @brief     GATT Server Characteristic Configuration UUID value
 */
#define BT_UUID_GATT_SCC_VAL 				0x2903

/**
 * @def       BT_UUID_GATT_SCC
 * @brief     GATT Server Characteristic Configuration
 */
#define BT_UUID_GATT_SCC \
	BT_UUID_DECLARE_16(BT_UUID_GATT_SCC_VAL)

/**
 * @def       BT_UUID_GATT_CPF_VAL
 * @brief     GATT Characteristic Presentation Format UUID value
 */
#define BT_UUID_GATT_CPF_VAL				0x2904

/**
 * @def       BT_UUID_GATT_CPF
 * @brief     GATT Characteristic Presentation Format
 */
#define BT_UUID_GATT_CPF \
	BT_UUID_DECLARE_16(BT_UUID_GATT_CPF_VAL)

/**
 * @def       BT_UUID_GATT_CAF_VAL
 * @brief     GATT Characteristic Aggregated Format UUID value
 */
#define BT_UUID_GATT_CAF_VAL 				0x2905

/**
 * @def   	  RTK_BT_GATT_INTERNAL
 * @brief     This attribute will be handle internally (Only in BT stack).
 */
#define RTK_BT_GATT_INTERNAL                0

/**
 * @def       RTK_BT_GATT_APP
 * @brief     This attribute will be handle by User APP (ALL read/write will have a callback).
 */
#define RTK_BT_GATT_APP                     1

/**
 * @def       RTK_BT_GATT_CHRC_BROADCAST
 * @brief     Characteristic broadcast property.
 *            If set, permits broadcasts of the Characteristic Value using Server
 *            Characteristic Configuration Descriptor.
 */
#define RTK_BT_GATT_CHRC_BROADCAST			0x01

/**
 * @def       RTK_BT_GATT_CHRC_READ
 * @brief     Characteristic read property.
 *            If set, permits reads of the Characteristic Value.
 */
#define RTK_BT_GATT_CHRC_READ				0x02

/**
 * @def       RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP
 * @brief     Characteristic write without response property.
 *            If set, permits write of the Characteristic Value without response.
 */
#define RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP	0x04

/**
 * @def       RTK_BT_GATT_CHRC_WRITE
 * @brief     Characteristic write with response property.
 *            If set, permits write of the Characteristic Value with response.
 */
#define RTK_BT_GATT_CHRC_WRITE				0x08

/**
 * @def       RTK_BT_GATT_CHRC_NOTIFY
 * @brief     Characteristic notify property.
 *            If set, permits notifications of a Characteristic Value without acknowledgment.
 */
#define RTK_BT_GATT_CHRC_NOTIFY				0x10

/**
 * @def       RTK_BT_GATT_CHRC_INDICATE
 * @brief     Characteristic indicate property.
 *            If set, permits indications of a Characteristic Value with acknowledgment.
 */
#define RTK_BT_GATT_CHRC_INDICATE			0x20

/**
 * @def       RTK_BT_GATT_CHRC_AUTH
 * @brief     Characteristic Authenticated Signed Writes property.
 *            If set, permits signed writes to the Characteristic Value.
 */
#define RTK_BT_GATT_CHRC_AUTH				0x40

/**
 * @def   	  RTK_BT_GATT_CHRC_EXT_PROP
 * @brief     Characteristic Extended Properties property.
 *            If set, additional characteristic properties are defined in the
 *            Characteristic Extended Properties Descriptor.
 */
#define RTK_BT_GATT_CHRC_EXT_PROP			0x80

/**
 * @def       RTK_BT_GATT_CCC_NOTIFY
 * @brief     Client Characteristic Configuration Notification.
 *            If set, changes to Characteristic Value shall be notified.
 */
#define RTK_BT_GATT_CCC_NOTIFY				0x0001

/**
 * @def       RTK_BT_GATT_CCC_INDICATE
 * @brief     Client Characteristic Configuration Indication.
 *            If set, changes to Characteristic Value shall be indicated.
 */
#define RTK_BT_GATT_CCC_INDICATE			0x0002

/**
 * @def       RTK_BT_GATT_SCC_BROADCAST
 * @brief     Server Characteristic Configuration Broadcast
 *            If set, the characteristic value shall be broadcast in the advertising data
 *            when the server is advertising.
 */
#define RTK_BT_GATT_SCC_BROADCAST			0x0001

/**
 * @enum  attr_perm_t
 * @brief GATT attribute permission bit field values.
 */
enum attr_perm_t
{
	RTK_BT_GATT_PERM_NONE          = 0,		/*!< No operations supported, e.g. for notify-only */
	RTK_BT_GATT_PERM_READ          = BIT0,	/*!< Attribute read permission. */
	RTK_BT_GATT_PERM_WRITE         = BIT1,	/*!< Attribute write permission. */
	RTK_BT_GATT_PERM_READ_ENCRYPT  = BIT2,	/*!< Attribute read permission with encryption.*/
	RTK_BT_GATT_PERM_WRITE_ENCRYPT = BIT3,	/*!< Attribute write permission with encryption. */
	RTK_BT_GATT_PERM_READ_AUTHEN   = BIT4,	/*! Attribute read permission with authentication. */
	RTK_BT_GATT_PERM_WRITE_AUTHEN  = BIT5,	/*!< Attribute write permission with authentication. */
	RTK_BT_GATT_PERM_PREPARE_WRITE = BIT6,	/*!< Attribute prepare write permission. */
};

/**
 * @typedef   rtk_bt_uuid_type_t
 * @brief     Bluetooth UUID type definition.
 */
typedef enum
{
	BT_UUID_TYPE_16,     /*!< UUID type 16-bit */
	BT_UUID_TYPE_32,     /*!< UUID type 32-bit */
	BT_UUID_TYPE_128,    /*!< UUID type 128-bit */
} rtk_bt_uuid_type_t;

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_ATT_DEFS_H__ */