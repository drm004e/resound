#!/bin/bash


echo '#ifndef __VERSION_STRING_HPP__' > version_string.hpp
echo '#define __VERSION_STRING_HPP__' >> version_string.hpp

echo '#define __VERSION_STRING__ "Resound version 0.1.'`svnversion`', built from '`svn info | grep "URL"`'"' >> version_string.hpp

echo '#endif'  >> version_string.hpp
