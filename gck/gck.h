/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* gck.h - the GObject PKCS#11 wrapper library

   Copyright (C) 2008, Stefan Walter

   The Gnome Keyring Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Keyring Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Author: Stef Walter <nielsen@memberwebs.com>
*/

#ifndef GCK_H
#define GCK_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "pkcs11.h"

G_BEGIN_DECLS

#define             GCK_VENDOR_CODE                         0x47503131 /* GP11 */

/* An error code which results from a failure to load the PKCS11 module */
#define             CKR_GCK_MODULE_PROBLEM                  (CKR_VENDOR_DEFINED | (GCK_VENDOR_CODE + 1))

#define             GCK_ERROR                               (gck_get_error_quark ())

GQuark              gck_get_error_quark                     (void);

GList*              gck_list_ref_copy                       (GList *reflist);

void                gck_list_unref_free                     (GList *reflist);

const gchar*        gck_message_from_rv                     (CK_RV rv);

gchar*              gck_string_from_chars                   (const guchar *data, gsize max);

typedef gpointer    (*GckAllocator)                         (gpointer data, gsize length);

typedef struct GckMechanism {
	gulong type;
	gpointer parameter;
	gulong n_parameter;
} GckMechanism;

GckMechanism*       gck_mechanism_new                       (gulong type);

GckMechanism*       gck_mechanism_new_with_param            (gulong type,
                                                             gconstpointer parameter,
                                                             gulong n_parameter);

GckMechanism*       gck_mechanism_ref                       (GckMechanism* mech);

void                gck_mechanism_unref                     (GckMechanism* mech);

typedef struct GckAttribute {
	gulong type;
	gpointer value;
	gulong length;
} GckAttribute;

#define GCK_BOOLEAN  ((gssize)-1)
#define GCK_ULONG    ((gssize)-2)
#define GCK_STRING   ((gssize)-3)
#define GCK_DATE     ((gssize)-4)

#define GCK_INVALID G_MAXULONG

enum {
	GCK_AUTHENTICATE_TOKENS = 2,
	GCK_AUTHENTICATE_OBJECTS = 4
};

void                gck_attribute_init                      (GckAttribute *attr,
                                                             gulong attr_type,
                                                             gconstpointer value,
                                                             gsize length);

void                gck_attribute_init_invalid              (GckAttribute *attr,
                                                             gulong attr_type);

void                gck_attribute_init_empty                (GckAttribute *attr,
                                                             gulong attr_type);

void                gck_attribute_init_boolean              (GckAttribute *attr,
                                                             gulong attr_type,
                                                             gboolean value);

void                gck_attribute_init_date                 (GckAttribute *attr,
                                                             gulong attr_type,
                                                             const GDate *value);

void                gck_attribute_init_ulong                (GckAttribute *attr,
                                                             gulong attr_type,
                                                             gulong value);

void                gck_attribute_init_string               (GckAttribute *attr,
                                                             gulong attr_type,
                                                             const gchar *value);

void                gck_attribute_init_copy                 (GckAttribute *dest,
                                                             const GckAttribute *src);

GckAttribute*       gck_attribute_new                       (gulong attr_type,
                                                             gpointer value,
                                                             gsize length);

GckAttribute*       gck_attribute_new_invalid               (gulong attr_type);

GckAttribute*       gck_attribute_new_empty                 (gulong attr_type);

GckAttribute*       gck_attribute_new_boolean               (gulong attr_type,
                                                             gboolean value);

GckAttribute*       gck_attribute_new_date                  (gulong attr_type,
                                                             const GDate *value);

GckAttribute*       gck_attribute_new_ulong                 (gulong attr_type,
                                                             gulong value);

GckAttribute*       gck_attribute_new_string                (gulong attr_type,
                                                             const gchar *value);

gboolean            gck_attribute_is_invalid                (GckAttribute *attr);

gboolean            gck_attribute_get_boolean               (GckAttribute *attr);

gulong              gck_attribute_get_ulong                 (GckAttribute *attr);

gchar*              gck_attribute_get_string                (GckAttribute *attr);

void                gck_attribute_get_date                  (GckAttribute *attr,
                                                             GDate* value);

GckAttribute*       gck_attribute_dup                       (GckAttribute *attr);

void                gck_attribute_clear                     (GckAttribute *attr);

void                gck_attribute_free                      (GckAttribute *attr);


typedef struct _GckAttributes GckAttributes;

#define             GCK_TYPE_ATTRIBUTES                     (gck_attributes_get_boxed_type ())

GType               gck_attributes_get_boxed_type           (void) G_GNUC_CONST;

GckAttributes*      gck_attributes_new                      (void);

GckAttributes*      gck_attributes_new_empty                (gulong attr_type,
                                                             ...);

GckAttributes*      gck_attributes_new_full                 (GckAllocator allocator);

GckAttributes*      gck_attributes_newv                     (gulong attr_type,
                                                             ...);

GckAttributes*      gck_attributes_new_valist               (GckAllocator allocator,
                                                             va_list va);

GckAttribute*       gck_attributes_at                       (GckAttributes *attrs,
                                                             guint index);

GckAttribute*       gck_attributes_add                      (GckAttributes *attrs,
                                                             GckAttribute *attr);

GckAttribute*       gck_attributes_add_data                 (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gconstpointer value,
                                                             gsize length);

GckAttribute*       gck_attributes_add_invalid              (GckAttributes *attrs,
                                                             gulong attr_type);

GckAttribute*       gck_attributes_add_empty                (GckAttributes *attrs,
                                                             gulong attr_type);

GckAttribute*       gck_attributes_add_boolean              (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gboolean value);

GckAttribute*       gck_attributes_add_string               (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             const gchar *value);

GckAttribute*       gck_attributes_add_date                 (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             const GDate *value);

GckAttribute*       gck_attributes_add_ulong                (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gulong value);

GckAttribute*       gck_attributes_find                     (GckAttributes *attrs,
                                                             gulong attr_type);

gboolean            gck_attributes_find_boolean             (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gboolean *value);

gboolean            gck_attributes_find_ulong               (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gulong *value);

gboolean            gck_attributes_find_string              (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             gchar **value);

gboolean            gck_attributes_find_date                (GckAttributes *attrs,
                                                             gulong attr_type,
                                                             GDate *value);

gulong              gck_attributes_count                    (GckAttributes *attrs);

GckAttributes*      gck_attributes_ref                      (GckAttributes *attrs);

void                gck_attributes_unref                    (GckAttributes *attrs);

/* -------------------------------------------------------------------------
 * FORWARDS
 */
typedef struct _GckSlot GckSlot;
typedef struct _GckModule GckModule;
typedef struct _GckSession GckSession;
typedef struct _GckObject GckObject;

typedef gboolean    (*GckObjectForeachFunc)                (GckObject *object, gpointer user_data);

/* -------------------------------------------------------------------------
 * MODULE
 */

typedef struct _GckModuleInfo {
	guint8 pkcs11_version_major;
	guint8 pkcs11_version_minor;

	gchar *manufacturer_id;
	gulong flags;

	gchar *library_description;
	guint8 library_version_major;
	guint8 library_version_minor;
} GckModuleInfo;

void                gck_module_info_free                   (GckModuleInfo *module_info);

#define GCK_TYPE_MODULE             (gck_module_get_type())
#define GCK_MODULE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GCK_TYPE_MODULE, GckModule))
#define GCK_MODULE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GCK_TYPE_MODULE, GckModule))
#define GCK_IS_MODULE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCK_TYPE_MODULE))
#define GCK_IS_MODULE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GCK_TYPE_MODULE))
#define GCK_MODULE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GCK_TYPE_MODULE, GckModuleClass))

typedef struct _GckModuleClass GckModuleClass;

struct _GckModule {
	GObject parent;
	gpointer reserved[4];
};

struct _GckModuleClass {
	GObjectClass parent;

	gboolean (*authenticate_slot) (GckModule *self, GckSlot *slot, gchar *label, gchar **password);

	gboolean (*authenticate_object) (GckModule *self, GckObject *object, gchar *label, gchar **password);

	gpointer reserved[8];
};

GType                 gck_module_get_type                     (void) G_GNUC_CONST;

GckModule*            gck_module_new                          (CK_FUNCTION_LIST_PTR funcs);

GckModule*            gck_module_initialize                   (const gchar *path,
                                                               gpointer reserved,
                                                               GError **err);

gboolean              gck_module_equal                        (gconstpointer module1,
                                                               gconstpointer module2);

guint                 gck_module_hash                         (gconstpointer module);

const gchar*          gck_module_get_path                     (GckModule *self);

CK_FUNCTION_LIST_PTR  gck_module_get_functions                (GckModule *self);

GckModuleInfo*        gck_module_get_info                     (GckModule *self);

GList*                gck_module_get_slots                    (GckModule *self,
                                                               gboolean token_present);

guint                 gck_module_get_options                  (GckModule *self);

void                  gck_module_set_options                  (GckModule *self,
                                                               guint options);

void                  gck_module_add_options                  (GckModule *self,
                                                               guint options);

GList*                gck_modules_get_slots                   (GList *modules,
                                                               gboolean token_present);

gboolean              gck_modules_enumerate_objects           (GList *modules,
                                                               GckObjectForeachFunc func,
                                                               gpointer user_data,
                                                               ...);

gboolean              gck_modules_enumerate_objects_full      (GList *modules,
                                                               GckAttributes *attrs,
                                                               guint session_flags,
                                                               GCancellable *cancellable,
                                                               GckObjectForeachFunc func,
                                                               gpointer user_data,
                                                               GError **error);

#ifdef UNIMPLEMENTED
void                  gck_modules_enumerate_objects_async     (GList *modules,
                                                               GckAttributes *attrs,
                                                               guint session_flags,
                                                               GckObjectForeachFunc func,
                                                               GCancellable *cancellable,
                                                               GAsyncReadyCallback callback,
                                                               gpointer user_data);

GckObject*            gck_modules_enumerate_objects_next      (GList *modules,
                                                               GAsyncResult *res,
                                                               GError **error);

void                  gck_modules_enumerate_objects_finish    (GList *modules,
                                                               GAsyncResult *res,
                                                               GError **error);
#endif

enum {
	GCK_IS_STRING = -1,
	GCK_IS_BOOLEAN = -2,
	GCK_IS_DATE = -3,
	GCK_IS_ULONG = -4
};

/* ------------------------------------------------------------------------
 * SLOT
 */

typedef struct _GckSlotInfo {
	gchar *slot_description;
	gchar *manufacturer_id;
	gulong flags;
	guint8 hardware_version_major;
	guint8 hardware_version_minor;
	guint8 firmware_version_major;
	guint8 firmware_version_minor;
} GckSlotInfo;

void                gck_slot_info_free                      (GckSlotInfo *slot_info);

typedef struct _GckTokenInfo {
	gchar *label;
	gchar *manufacturer_id;
	gchar *model;
	gchar *serial_number;
	gulong flags;
	glong max_session_count;
	glong session_count;
	glong max_rw_session_count;
	glong rw_session_count;
	glong max_pin_len;
	glong min_pin_len;
	glong total_public_memory;
	glong free_public_memory;
	glong total_private_memory;
	glong free_private_memory;
	guint8 hardware_version_major;
	guint8 hardware_version_minor;
	guint8 firmware_version_major;
	guint8 firmware_version_minor;
	gint64 utc_time;
} GckTokenInfo;

void                gck_token_info_free                     (GckTokenInfo *token_info);

typedef struct _GckMechanismInfo {
	gulong min_key_size;
	gulong max_key_size;
	gulong flags;
} GckMechanismInfo;

void                gck_mechanism_info_free                 (GckMechanismInfo *mech_info);

typedef GArray GckMechanisms;

#define             gck_mechanisms_length(a)                ((a)->len)

#define             gck_mechanisms_at(a, i)                 (g_array_index(a, CK_MECHANISM_TYPE, i))

#define             gck_mechanisms_free(a)                  (g_array_free(a, TRUE))

gboolean            gck_mechanisms_check                    (GckMechanisms *mechanisms,
                                                             ...);

#define GCK_TYPE_SLOT             (gck_slot_get_type())
#define GCK_SLOT(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GCK_TYPE_SLOT, GckSlot))
#define GCK_SLOT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GCK_TYPE_SLOT, GckSlot))
#define GCK_IS_SLOT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCK_TYPE_SLOT))
#define GCK_IS_SLOT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GCK_TYPE_SLOT))
#define GCK_SLOT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GCK_TYPE_SLOT, GckSlotClass))

typedef struct _GckSlotClass GckSlotClass;

struct _GckSlot {
	GObject parent;
	gpointer reserved[4];
};

struct _GckSlotClass {
	GObjectClass parent;
	gpointer reserved[9];
};

GType               gck_slot_get_type                       (void) G_GNUC_CONST;

gboolean            gck_slot_equal                          (gconstpointer slot1,
                                                             gconstpointer slot2);

guint               gck_slot_hash                           (gconstpointer slot);

GckModule*          gck_slot_get_module                     (GckSlot *self);

CK_SLOT_ID          gck_slot_get_handle                     (GckSlot *self);

GckSlotInfo*        gck_slot_get_info                       (GckSlot *self);

GckTokenInfo*       gck_slot_get_token_info                  (GckSlot *self);

GckMechanisms*      gck_slot_get_mechanisms                 (GckSlot *self);

GckMechanismInfo*   gck_slot_get_mechanism_info             (GckSlot *self,
                                                             gulong mech_type);

gboolean            gck_slot_has_flags                      (GckSlot *self,
                                                             gulong flags);

#if UNIMPLEMENTED

gboolean            gck_slot_init_token                     (GckSlot *self,
                                                             const guchar *pin,
                                                             gsize length,
                                                             const gchar *label,
                                                             GError **err);


void                gck_slot_init_token_async               (GckSlot *self,
                                                             const guchar *pin,
                                                             gsize length,
                                                             const gchar *label,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_slot_init_token_finish              (GckSlot *self,
                                                             GAsyncResult *result,
                                                             GError **err);

#endif /* UNIMPLEMENTED */

GckSession*         gck_slot_open_session                   (GckSlot *self,
                                                             gulong flags,
                                                             GError **err);

GckSession*         gck_slot_open_session_full              (GckSlot *self,
                                                             gulong flags,
                                                             gpointer app_data,
                                                             CK_NOTIFY notify,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_slot_open_session_async             (GckSlot *self,
                                                             gulong flags,
                                                             gpointer app_data,
                                                             CK_NOTIFY notify,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GckSession*         gck_slot_open_session_finish            (GckSlot *self,
                                                             GAsyncResult *result,
                                                             GError **err);

/* ------------------------------------------------------------------------
 * SESSION
 */

typedef struct _GckSessionInfo {
	gulong slot_id;
	gulong state;
	gulong flags;
	gulong device_error;
} GckSessionInfo;

void                gck_session_info_free                  (GckSessionInfo *session_info);

#define GCK_TYPE_SESSION             (gck_session_get_type())
#define GCK_SESSION(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GCK_TYPE_SESSION, GckSession))
#define GCK_SESSION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GCK_TYPE_SESSION, GckSession))
#define GCK_IS_SESSION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCK_TYPE_SESSION))
#define GCK_IS_SESSION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GCK_TYPE_SESSION))
#define GCK_SESSION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GCK_TYPE_SESSION, GckSessionClass))

typedef struct _GckSessionClass GckSessionClass;

struct _GckSession {
	GObject parent;
	gpointer reserved[4];
};

struct _GckSessionClass {
	GObjectClass parent;

	gboolean (*discard_handle) (GckSession *session, CK_SESSION_HANDLE handle);

	gpointer reserved[8];
};

GType               gck_session_get_type                    (void) G_GNUC_CONST;

GckSession*         gck_session_from_handle                 (GckSlot *slot,
                                                             CK_SESSION_HANDLE handle);

GckModule*          gck_session_get_module                  (GckSession *self);

GckSlot*            gck_session_get_slot                    (GckSession *self);

CK_SESSION_HANDLE   gck_session_get_handle                  (GckSession *self);

GckSessionInfo*     gck_session_get_info                    (GckSession *self);

gboolean            gck_session_init_pin                    (GckSession *self,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GError **err);

gboolean            gck_session_init_pin_full               (GckSession *self,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_init_pin_async              (GckSession *self,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_session_init_pin_finish             (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

gboolean            gck_session_set_pin                     (GckSession *self,
                                                             const guchar *old_pin,
                                                             gsize n_old_pin,
                                                             const guchar *new_pin,
                                                             gsize n_new_pin,
                                                             GError **err);

gboolean            gck_session_set_pin_full                (GckSession *self,
                                                             const guchar *old_pin,
                                                             gsize n_old_pin,
                                                             const guchar *new_pin,
                                                             gsize n_new_pin,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_set_pin_async               (GckSession *self,
                                                             const guchar *old_pin,
                                                             gsize n_old_pin,
                                                             const guchar *new_pin,
                                                             gsize n_new_pin,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_session_set_pin_finish              (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

gboolean            gck_session_login                       (GckSession *self,
                                                             gulong user_type,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GError **err);

gboolean            gck_session_login_full                  (GckSession *self,
                                                             gulong user_type,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_login_async                 (GckSession *self,
                                                             gulong user_type,
                                                             const guchar *pin,
                                                             gsize n_pin,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_session_login_finish                (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

gboolean            gck_session_logout                      (GckSession *self,
                                                             GError **err);

gboolean            gck_session_logout_full                 (GckSession *self,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_logout_async                (GckSession *self,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_session_logout_finish               (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

GckObject*          gck_session_create_object               (GckSession *self,
                                                             GError **err,
                                                             ...);

GckObject*          gck_session_create_object_full          (GckSession *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_create_object_async         (GckSession *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GckObject*          gck_session_create_object_finish        (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

GList*              gck_session_find_objects                (GckSession *self,
                                                             GError **err,
                                                             ...);

GList*              gck_session_find_objects_full           (GckSession *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_find_objects_async          (GckSession *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GList*              gck_session_find_objects_finish         (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err);

#if UNIMPLEMENTED

GckObject*          gck_session_generate_key                (GckSession *self,
                                                             GckMechanism *mechanism,
                                                             GError **err,
                                                             ...);

void                gck_session_generate_key_async          (GckSession *self,
                                                             GckMechanism *mechanism,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data,
                                                             ...);

GckObject*          gck_session_generate_key_finish         (GckSession *self,
                                                             GAsyncResult *result,
                                                             GError **err,
                                                             ...);

#endif /* UNIMPLEMENTED */

gboolean            gck_session_generate_key_pair_full      (GckSession *self,
                                                             GckMechanism *mechanism,
                                                             GckAttributes *public_attrs,
                                                             GckAttributes *private_attrs,
                                                             GckObject **public_key,
                                                             GckObject **private_key,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_session_generate_key_pair_async     (GckSession *self,
                                                             GckMechanism *mechanism,
                                                             GckAttributes *public_attrs,
                                                             GckAttributes *private_attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_session_generate_key_pair_finish    (GckSession *self,
                                                             GAsyncResult *result,
                                                             GckObject **public_key,
                                                             GckObject **private_key,
                                                             GError **err);

guchar*             gck_session_encrypt                      (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_encrypt_full                 (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_encrypt_async                (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_encrypt_finish               (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_decrypt                      (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_decrypt_full                 (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_decrypt_async                (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_decrypt_finish               (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

#if UNIMPLEMENTED

guchar*             gck_session_digest                       (GckSession *self,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_digest_full                  (GckSession *self,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_digest_async                 (GckSession *self,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_digest_finish                (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

#endif /* UNIMPLEMENTED */

guchar*             gck_session_sign                         (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_sign_full                    (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_sign_async                   (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_sign_finish                  (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

#if UNIMPLEMENTED

guchar*             gck_session_sign_recover                 (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_sign_recover_full            (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_sign_recover_async           (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_sign_recover_finish          (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

#endif /* UNIMPLEMENTED */

gboolean            gck_session_verify                       (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              const guchar *signature,
                                                              gsize n_signature,
                                                              GError **err);

gboolean            gck_session_verify_full                  (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              const guchar *signature,
                                                              gsize n_signature,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_verify_async                 (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              const guchar *signature,
                                                              gsize n_signature,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

gboolean            gck_session_verify_finish                (GckSession *self,
                                                              GAsyncResult *result,
                                                              GError **err);

#if UNIMPLEMENTED

guchar*             gck_session_verify_recover               (GckSession *self,
                                                              GckObject *key,
                                                              gulong mech_type,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GError **err);

guchar*             gck_session_verify_recover_full          (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_verify_recover_async         (GckSession *self,
                                                              GckObject *key,
                                                              GckMechanism *mechanism,
                                                              const guchar *input,
                                                              gsize n_input,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

guchar*             gck_session_verify_recover_finish        (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

#endif /* UNIMPLEMENTED */

gpointer            gck_session_wrap_key                     (GckSession *self,
                                                              GckObject *wrapper,
                                                              gulong mech_type,
                                                              GckObject *wrapped,
                                                              gsize *n_result,
                                                              GError **err);

gpointer            gck_session_wrap_key_full                (GckSession *self,
                                                              GckObject *wrapper,
                                                              GckMechanism *mechanism,
                                                              GckObject *wrapped,
                                                              gsize *n_result,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_wrap_key_async               (GckSession *self,
                                                              GckObject *wrapper,
                                                              GckMechanism *mechanism,
                                                              GckObject *wrapped,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

gpointer            gck_session_wrap_key_finish              (GckSession *self,
                                                              GAsyncResult *result,
                                                              gsize *n_result,
                                                              GError **err);

GckObject*          gck_session_unwrap_key                   (GckSession *self,
                                                              GckObject *wrapper,
                                                              gulong mech_type,
                                                              gconstpointer input,
                                                              gsize n_input,
                                                              GError **err,
                                                              ...);

GckObject*          gck_session_unwrap_key_full              (GckSession *self,
                                                              GckObject *wrapper,
                                                              GckMechanism *mechanism,
                                                              gconstpointer input,
                                                              gsize n_input,
                                                              GckAttributes *attrs,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_unwrap_key_async             (GckSession *self,
                                                              GckObject *wrapper,
                                                              GckMechanism *mechanism,
                                                              gconstpointer input,
                                                              gsize n_input,
                                                              GckAttributes *attrs,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

GckObject*          gck_session_unwrap_key_finish            (GckSession *self,
                                                              GAsyncResult *result,
                                                              GError **err);

GckObject*          gck_session_derive_key                   (GckSession *self,
                                                              GckObject *base,
                                                              gulong mech_type,
                                                              GError **err,
                                                              ...);

GckObject*          gck_session_derive_key_full              (GckSession *self,
                                                              GckObject *base,
                                                              GckMechanism *mechanism,
                                                              GckAttributes *attrs,
                                                              GCancellable *cancellable,
                                                              GError **err);

void                gck_session_derive_key_async             (GckSession *self,
                                                              GckObject *base,
                                                              GckMechanism *mechanism,
                                                              GckAttributes *attrs,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);

GckObject*          gck_session_derive_key_finish            (GckSession *self,
                                                              GAsyncResult *result,
                                                              GError **err);

/* ------------------------------------------------------------------------
 * OBJECT
 */

#define GCK_TYPE_OBJECT             (gck_object_get_type())
#define GCK_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GCK_TYPE_OBJECT, GckObject))
#define GCK_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GCK_TYPE_OBJECT, GckObject))
#define GCK_IS_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GCK_TYPE_OBJECT))
#define GCK_IS_OBJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GCK_TYPE_OBJECT))
#define GCK_OBJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), GCK_TYPE_OBJECT, GckObjectClass))

typedef struct _GckObjectClass GckObjectClass;

struct _GckObject {
	GObject parent;
	gpointer reserved[4];
};

struct _GckObjectClass {
	GObjectClass parent;
	gpointer reserved[8];
};

GType               gck_object_get_type                     (void) G_GNUC_CONST;

GckObject*          gck_object_from_handle                  (GckSession *session,
                                                             CK_OBJECT_HANDLE handle);

GList*              gck_objects_from_handle_array           (GckSession *session,
                                                             CK_OBJECT_HANDLE_PTR handles,
                                                             CK_ULONG n_handles);

gboolean            gck_object_equal                        (gconstpointer object1,
                                                             gconstpointer object2);

guint               gck_object_hash                         (gconstpointer object);

GckModule*          gck_object_get_module                   (GckObject *self);

CK_OBJECT_HANDLE    gck_object_get_handle                   (GckObject *self);

GckSession*         gck_object_get_session                  (GckObject *self);

#ifdef UNIMPLEMENTED

GckObject*          gck_object_copy                         (GckObject *self,
                                                             GError **err);

GckObject*          gck_object_copy_full                    (GckObject *self,
                                                             GckAttributes *additional,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_copy_async                   (GckObject *self,
                                                             GckAttributes *additional,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GckObject*          gck_object_copy_finish                  (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

#endif /* UNIMPLEMENTED */

gboolean            gck_object_destroy                      (GckObject *self,
                                                             GError **err);

gboolean            gck_object_destroy_full                 (GckObject *self,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_destroy_async                (GckObject *self,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_object_destroy_finish               (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

#if UNIMPLEMENTED

gssize              gck_object_get_size                     (GckObject *self,
                                                             GError **err);

gssize              gck_object_get_size_full                (GckObject *self,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_get_size_async               (GckObject *self,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gssize              gck_object_get_size_finish              (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

#endif /* UNIMPLEMENTED */

gboolean            gck_object_set                          (GckObject *self,
                                                             GError **err,
                                                             ...);

gboolean            gck_object_set_full                     (GckObject *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_set_async                    (GckObject *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_object_set_finish                   (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

GckAttributes*      gck_object_get                          (GckObject *self,
                                                             GError **err,
                                                             ...);

GckAttributes*      gck_object_get_full                     (GckObject *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_get_async                    (GckObject *self,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GckAttributes*      gck_object_get_finish                   (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

gpointer            gck_object_get_data                     (GckObject *self,
                                                             gulong attr_type,
                                                             gsize *n_data,
                                                             GError **err);

gpointer            gck_object_get_data_full                (GckObject *self,
                                                             gulong attr_type,
                                                             GckAllocator allocator,
                                                             GCancellable *cancellable,
                                                             gsize *n_data,
                                                             GError **err);

void                gck_object_get_data_async               (GckObject *self,
                                                             gulong attr_type,
                                                             GckAllocator allocator,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gpointer            gck_object_get_data_finish              (GckObject *self,
                                                             GAsyncResult *result,
                                                             gsize *n_data,
                                                             GError **err);

gboolean            gck_object_set_template                 (GckObject *self,
                                                             gulong attr_type,
                                                             GckAttributes *attrs,
                                                             GError **err);

gboolean            gck_object_set_template_full            (GckObject *self,
                                                             gulong attr_type,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_set_template_async           (GckObject *self,
                                                             gulong attr_type,
                                                             GckAttributes *attrs,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

gboolean            gck_object_set_template_finish          (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

GckAttributes*      gck_object_get_template                 (GckObject *self,
                                                             gulong attr_type,
                                                             GError **err);

GckAttributes*      gck_object_get_template_full            (GckObject *self,
                                                             gulong attr_type,
                                                             GCancellable *cancellable,
                                                             GError **err);

void                gck_object_get_template_async           (GckObject *self,
                                                             gulong attr_type,
                                                             GCancellable *cancellable,
                                                             GAsyncReadyCallback callback,
                                                             gpointer user_data);

GckAttributes*      gck_object_get_template_finish          (GckObject *self,
                                                             GAsyncResult *result,
                                                             GError **err);

G_END_DECLS

#endif /* GCK_H */
