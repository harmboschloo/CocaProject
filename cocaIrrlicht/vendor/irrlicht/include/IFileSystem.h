// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_FILE_SYSTEM_H_INCLUDED__
#define __I_FILE_SYSTEM_H_INCLUDED__

#include "IReferenceCounted.h"
#include "IXMLReader.h"
#include "IFileArchive.h"

namespace irr
{
namespace video
{
	class IVideoDriver;
} // end namespace video
namespace io
{

class IReadFile;
class IWriteFile;
class IFileList;
class IXMLWriter;
class IAttributes;


//! The FileSystem manages files and archives and provides access to them.
/** It manages where files are, so that modules which use the the IO do not
need to know where every file is located. A file could be in a .zip-Archive or
as file on disk, using the IFileSystem makes no difference to this. */
class IFileSystem : public virtual IReferenceCounted
{
public:

	//! Opens a file for read access.
	/** \param filename: Name of file to open.
	\return Returns a pointer to the created file interface.
	The returned pointer should be dropped when no longer needed.
	See IReferenceCounted::drop() for more information. */
	virtual IReadFile* createAndOpenFile(const core::string<c16>& filename) =0;

	//! Creates an IReadFile interface for accessing memory like a file.
	/** This allows you to use a pointer to memory where an IReadFile is requested.
	\param memory: A pointer to the start of the file in memory
	\param len: The length of the memory in bytes
	\param fileName: The name given to this file
	\param deleteMemoryWhenDropped: True if the memory should be deleted
	along with the IReadFile when it is dropped.
	\return Returns a pointer to the created file interface.
	The returned pointer should be dropped when no longer needed.
	See IReferenceCounted::drop() for more information.
	*/
	virtual IReadFile* createMemoryReadFile(void* memory, s32 len, const core::string<c16>& fileName, bool deleteMemoryWhenDropped=false) =0;

	//! Creates an IReadFile interface for accessing files inside files.
	/** This is useful e.g. for archives.
	\param fileName: The name given to this file
	\param alreadyOpenedFile: Pointer to the enclosing file
	\param pos: Start of the file inside alreadyOpenedFile
	\param areaSize: The length of the file
	\return A pointer to the created file interface.
	The returned pointer should be dropped when no longer needed.
	See IReferenceCounted::drop() for more information.
	*/
	virtual IReadFile* createLimitReadFile(const core::string<c16>& fileName,
			IReadFile* alreadyOpenedFile, long pos, long areaSize) =0;

	//! Creates an IWriteFile interface for accessing memory like a file.
	/** This allows you to use a pointer to memory where an IWriteFile is requested.
		You are responsible for allocating enough memory.
	\param memory: A pointer to the start of the file in memory (allocated by you)
	\param len: The length of the memory in bytes
	\param fileName: The name given to this file
	\param deleteMemoryWhenDropped: True if the memory should be deleted
	along with the IWriteFile when it is dropped.
	\return Returns a pointer to the created file interface.
	The returned pointer should be dropped when no longer needed.
	See IReferenceCounted::drop() for more information.
	*/
	virtual IWriteFile* createMemoryWriteFile(void* memory, s32 len, const core::string<c16>& fileName, bool deleteMemoryWhenDropped=false) =0;


	//! Opens a file for write access.
	/** \param filename: Name of file to open.
	\param append: If the file already exist, all write operations are
	appended to the file.
	\return Returns a pointer to the created file interface. 0 is returned, if the
	file could not created or opened for writing.
	The returned pointer should be dropped when no longer needed.
	See IReferenceCounted::drop() for more information. */
	virtual IWriteFile* createAndWriteFile(const core::string<c16>& filename, bool append=false) =0;

	//! Adds an archive to the file system.
	/** After calling this, the Irrlicht Engine will search and open files directly from this archive too.
	This is useful for hiding data from the end user, speeding up file access and making it possible to
	access for example Quake3 .pk3 files, which are nothing different than .zip files.
	\param filename: Filename of the zip archive to add to the file system.
	\param ignoreCase: If set to true, files in the archive can be accessed without
	writing all letters in the right case.
	\param ignorePaths: If set to true, files in the added archive can be accessed
	without its complete path.
	\return Returns true if the archive was added successful, false if not. */
	virtual bool registerFileArchive(const core::string<c16>& filename, bool ignoreCase=true, bool ignorePaths=true) =0;

	//! Adds an external archive loader to the engine.
	virtual void addArchiveLoader(IArchiveLoader* loader) =0;

	//! return the amount of currently attached Archives
	virtual u32 getFileArchiveCount() const =0;

	//! removes an archive from the file system.
	virtual bool unregisterFileArchive(u32 index) =0;

	//! removes an archive from the file system.
	virtual bool unregisterFileArchive(const core::string<c16>& filename) =0;

	//! move the hirarchy of the filesystem. moves sourceIndex relative up or down
	virtual bool moveFileArchive(u32 sourceIndex, s32 relative) =0;

	//! get the Archive number index
	virtual IFileArchive* getFileArchive(u32 index) =0;

	//! Adds an zip archive to the file system.
	/** After calling this, the Irrlicht Engine will search and open files directly from this archive too.
	This is useful for hiding data from the end user, speeding up file access and making it possible to
	access for example Quake3 .pk3 files, which are nothing different than .zip files.
	\param filename: Filename of the zip archive to add to the file system.
	\param ignoreCase: If set to true, files in the archive can be accessed without
	writing all letters in the right case.
	\param ignorePaths: If set to true, files in the added archive can be accessed
	without its complete path.
	\return Returns true if the archive was added successful, false if not. */
	virtual bool addZipFileArchive(const c8* filename, bool ignoreCase=true, bool ignorePaths=true)
	{
		return registerFileArchive(filename, ignoreCase, ignorePaths);
	}

	//! Adds an unzipped archive (or basedirectory with subdirectories..) to the file system.
	/** Useful for handling data which will be in a zip file
	\param filename: Filename of the unzipped zip archive base directory to add to the file system.
	\param ignoreCase: If set to true, files in the archive can be accessed without
	writing all letters in the right case.
	\param ignorePaths: If set to true, files in the added archive can be accessed
	without its complete path.
	\return Returns true if the archive was added successful, false if not. */
	virtual bool addFolderFileArchive(const c8* filename, bool ignoreCase=true, bool ignorePaths=true)
	{
		return registerFileArchive(filename, ignoreCase, ignorePaths);
	}

	//! Adds an pak archive to the file system.
	/** After calling this, the Irrlicht Engine will search and open files directly from this archive too.
	This is useful for hiding data from the end user, speeding up file access and making it possible to
	access for example Quake2/KingPin/Hexen2 .pak files
	\param filename: Filename of the pak archive to add to the file system.
	\param ignoreCase: If set to true, files in the archive can be accessed without
	writing all letters in the right case.
	\param ignorePaths: If set to true, files in the added archive can be accessed
	without its complete path.(should not use with Quake2 paks
	\return Returns true if the archive was added successful, false if not. */
	virtual bool addPakFileArchive(const c8* filename, bool ignoreCase=true, bool ignorePaths=true)
	{
		return registerFileArchive(filename, ignoreCase, ignorePaths);
	}

	//! Get the current working directory.
	/** \return Current working directory as a string. */
	virtual const core::string<c16>& getWorkingDirectory() =0;

	//! Changes the current working directory.
	/** \param newDirectory: A string specifying the new working directory.
	The string is operating system dependent. Under Windows it has
	the form "<drive>:\<directory>\<sudirectory>\<..>". An example would be: "C:\Windows\"
	\return True if successful, otherwise false. */
	virtual bool changeWorkingDirectoryTo(const core::string<c16>& newDirectory) =0;

	//! Converts a relative path to an absolute (unique) path, resolving symbolic links if required
	/** \param filename Possibly relative filename begin queried.
	\result Absolute filename which points to the same file. */
	virtual core::string<c16> getAbsolutePath(const core::string<c16>& filename) const =0;

	//! Returns the directory a file is located in.
	/** \param filename: The file to get the directory from.
	\return String containing the directory of the file. */
	virtual core::string<c16> getFileDir(const core::string<c16>& filename) const =0;

	//! Returns the base part of a filename, i.e. the name without the directory part.
	/** If no directory is prefixed, the full name is returned.
	\param filename: The file to get the basename from
	\param keepExtension True if filename with extension is returned otherwise everything
	after the final '.' is removed as well. */
	virtual core::string<c16> getFileBasename(const core::string<c16>& filename, bool keepExtension=true) const =0;

	//! flatten a path and file name for example: "/you/me/../." becomes "/you"
	virtual core::string<c16>& flattenFilename(core::string<c16>& directory, const core::string<c16>& root="/") const =0;

	//! Creates a list of files and directories in the current working directory and returns it.
	/** \return a Pointer to the created IFileList is returned. After the list has been used
	it has to be deleted using its IFileList::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IFileList* createFileList() =0;

	//! Set the active type of file system.
	virtual EFileSystemType setFileListSystem(EFileSystemType listType) =0;

	//! Determines if a file exists and could be opened.
	/** \param filename is the string identifying the file which should be tested for existence.
	\return Returns true if file exists, and false if it does not exist or an error occured. */
	virtual bool existFile(const core::string<c16>& filename) const =0;

	//! Creates a XML Reader from a file which returns all parsed strings as wide characters (wchar_t*).
	/** Use createXMLReaderUTF8() if you prefer char* instead of wchar_t*. See IIrrXMLReader for
	more information on how to use the parser.
	\return 0, if file could not be opened, otherwise a pointer to the created
	IXMLReader is returned. After use, the reader
	has to be deleted using its IXMLReader::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLReader* createXMLReader(const core::string<c16>& filename) =0;

	//! Creates a XML Reader from a file which returns all parsed strings as wide characters (wchar_t*).
	/** Use createXMLReaderUTF8() if you prefer char* instead of wchar_t*. See IIrrXMLReader for
	more information on how to use the parser.
	\return 0, if file could not be opened, otherwise a pointer to the created
	IXMLReader is returned. After use, the reader
	has to be deleted using its IXMLReader::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLReader* createXMLReader(IReadFile* file) =0;

	//! Creates a XML Reader from a file which returns all parsed strings as ASCII/UTF-8 characters (char*).
	/** Use createXMLReader() if you prefer wchar_t* instead of char*. See IIrrXMLReader for
	more information on how to use the parser.
	\return 0, if file could not be opened, otherwise a pointer to the created
	IXMLReader is returned. After use, the reader
	has to be deleted using its IXMLReaderUTF8::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLReaderUTF8* createXMLReaderUTF8(const core::string<c16>& filename) =0;

	//! Creates a XML Reader from a file which returns all parsed strings as ASCII/UTF-8 characters (char*).
	/** Use createXMLReader() if you prefer wchar_t* instead of char*. See IIrrXMLReader for
	more information on how to use the parser.
	\return 0, if file could not be opened, otherwise a pointer to the created
	IXMLReader is returned. After use, the reader
	has to be deleted using its IXMLReaderUTF8::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file) =0;

	//! Creates a XML Writer from a file.
	/** \return 0, if file could not be opened, otherwise a pointer to the created
	IXMLWriter is returned. After use, the reader
	has to be deleted using its IXMLWriter::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLWriter* createXMLWriter(const core::string<c16>& filename) =0;

	//! Creates a XML Writer from a file.
	/** \return 0, if file could not be opened, otherwise a pointer to the created
	IXMLWriter is returned. After use, the reader
	has to be deleted using its IXMLWriter::drop() method.
	See IReferenceCounted::drop() for more information. */
	virtual IXMLWriter* createXMLWriter(IWriteFile* file) =0;

	//! Creates a new empty collection of attributes, usable for serialization and more.
	/** \param driver: Video driver to be used to load textures when specified as attribute values.
	Can be null to prevent automatic texture loading by attributes.
	\return Pointer to the created object.
	If you no longer need the object, you should call IAttributes::drop().
	See IReferenceCounted::drop() for more information. */
	virtual IAttributes* createEmptyAttributes(video::IVideoDriver* driver=0) =0;
};


} // end namespace io
} // end namespace irr

#endif

