#include "UnrealEnginePythonPrivatePCH.h"

#if WITH_EDITOR

static PyObject *py_ue_fassetdata_get_asset(ue_PyFAssetData *self, PyObject * args) {
	PyObject *ret = (PyObject *)ue_get_python_wrapper(self->asset_data.GetAsset());
	if (!ret) {
		return PyErr_Format(PyExc_Exception, "unable to get UObject from asset");
	}
	Py_INCREF(ret);
	return ret;
}

static PyObject *py_ue_fassetdata_is_asset_loaded(ue_PyFAssetData *self, PyObject * args) {
	if (self->asset_data.IsAssetLoaded())
		Py_RETURN_TRUE;
	Py_RETURN_FALSE;
}

static PyMethodDef ue_PyFAssetData_methods[] = {
	{ "get_asset", (PyCFunction)py_ue_fassetdata_get_asset, METH_VARARGS, "" },
	{ "is_asset_loaded", (PyCFunction)py_ue_fassetdata_is_asset_loaded, METH_VARARGS, "" },
	{ NULL }  /* Sentinel */
};

static PyObject *py_ue_fassetdata_get_asset_class(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.AssetClass.ToString()));
}

static PyObject *py_ue_fassetdata_get_asset_name(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.AssetName.ToString()));
}

#if ENGINE_MINOR_VERSION < 17
static PyObject *py_ue_fassetdata_get_group_names(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.GroupNames.ToString()));
}
#endif

static PyObject *py_ue_fassetdata_get_object_path(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.ObjectPath.ToString()));
}

static PyObject *py_ue_fassetdata_get_package_flags(ue_PyFAssetData *self, void *closure) {
	return PyLong_FromUnsignedLong(self->asset_data.PackageFlags);
}

static PyObject *py_ue_fassetdata_get_package_name(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.PackageName.ToString()));
}

static PyObject *py_ue_fassetdata_get_package_path(ue_PyFAssetData *self, void *closure) {
	return PyUnicode_FromString(TCHAR_TO_UTF8(*self->asset_data.PackagePath.ToString()));
}

static PyObject *py_ue_fassetdata_get_tags_and_values(ue_PyFAssetData *self, void *closure) {
	PyObject *ret = PyDict_New();
	for (auto It = self->asset_data.TagsAndValues.CreateConstIterator(); It; ++It)
	{
		PyDict_SetItem(ret,
			PyUnicode_FromString(TCHAR_TO_UTF8(*It->Key.ToString())),
			PyUnicode_FromString(TCHAR_TO_UTF8(*It->Value)));
	}
	return ret;
}

static PyGetSetDef ue_PyFAssetData_getseters[] = {
	{ (char *)"asset_class", (getter)py_ue_fassetdata_get_asset_class, nullptr, (char *)"asset_class" },
	{ (char *)"asset_name", (getter)py_ue_fassetdata_get_asset_name, nullptr, (char *)"asset_name" },
#if ENGINE_MINOR_VERSION < 17
	{ (char *)"group_names", (getter)py_ue_fassetdata_get_group_names, nullptr, (char *)"group_names" },
#endif
	{ (char *)"object_path",(getter)py_ue_fassetdata_get_object_path, nullptr, (char *)"object_path" },
	{ (char *)"package_flags",(getter)py_ue_fassetdata_get_package_flags, nullptr, (char *)"package_flags" },
	{ (char *)"package_name", (getter)py_ue_fassetdata_get_package_name, nullptr, (char *)"package_name" },
	{ (char *)"package_path", (getter)py_ue_fassetdata_get_package_path, nullptr, (char *)"package_path" },
	{ (char *)"tags_and_values", (getter)py_ue_fassetdata_get_tags_and_values, nullptr, (char *)"tags_and_values" },
	{ NULL }  /* Sentinel */
};

static int ue_py_fassetdata_init(ue_PyFAssetData *self, PyObject *args, PyObject *kwargs) {
	// avoid FAssetData manual creation
	return -1;
}

static PyObject *ue_PyFAssetData_str(ue_PyFAssetData *self)
{
	return PyUnicode_FromFormat("<unreal_engine.FAssetData '%s'>",
		TCHAR_TO_UTF8(*self->asset_data.GetExportTextName()));
}

static PyTypeObject ue_PyFAssetDataType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.FAssetData", /* tp_name */
	sizeof(ue_PyFAssetData),    /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,   /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	(reprfunc)ue_PyFAssetData_str,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,        /* tp_flags */
	"Unreal Engine FAssetData", /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	ue_PyFAssetData_methods,    /* tp_methods */
	0,   /* tp_members */
	ue_PyFAssetData_getseters,                         /* tp_getset */
};

void ue_python_init_fassetdata(PyObject *ue_module) {
	ue_PyFAssetDataType.tp_new = PyType_GenericNew;;
	ue_PyFAssetDataType.tp_init = (initproc)ue_py_fassetdata_init;
	if (PyType_Ready(&ue_PyFAssetDataType) < 0)
		return;

	Py_INCREF(&ue_PyFAssetDataType);
	PyModule_AddObject(ue_module, "FAssetData", (PyObject *)&ue_PyFAssetDataType);
}

PyObject *py_ue_new_fassetdata(FAssetData asset_data) {
	ue_PyFAssetData *ret = (ue_PyFAssetData *)PyObject_New(ue_PyFAssetData, &ue_PyFAssetDataType);

	new(&ret->asset_data) FAssetData(asset_data);
	return (PyObject *)ret;
}

ue_PyFAssetData *py_ue_is_fassetdata(PyObject *obj) {
	if (!PyObject_IsInstance(obj, (PyObject *)&ue_PyFAssetDataType))
		return nullptr;
	return (ue_PyFAssetData *)obj;
}

#endif
