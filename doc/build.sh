#!/bin/bash


SOURCE_DIRECTORY_BLACK_LIST=("bin/" "build/" "tmp/")


usage() {
    echo "usage build.sh <Operation> [-d <Document>] [-f <Format>] [-b <BuildDirectory>]"
    echo ""
    echo "<Operation>:"
    echo "  - help:  Show this help"
    echo "  - build: Build HTML and/or PDF document"
    echo "  - clean: Remove HTML and/or PDF document"
    echo ""
    echo "<Document>:"
    echo "  Name of the document for which the operation should be performed."
    echo "  ALL is used as default."
    echo ""
    echo "<Format>:"
    echo "  - PDF:  PDF Format"
    echo "  - HTML: HTML Format"
    echo "  ALL is used as default"
    echo ""
    echo "<BuildDirectory>:"
    echo "  Name of the directory in which the documents are stored."
    echo "  If the parameter <BuildDirectory> is not specified, the document is saved in default directory ~/build."
    echo "  The default directory can be overwritten with the environment variable KONVEKTA_DOCUMENT_ROOT."
    echo ""
    echo "Structure of Document Directory:"
    echo "  <BuildDirectory>/<Document>/html"
    echo "  <BuildDirectory>/<Document>/pdf>"
    exit 1
}

#
# Check number of input arguments
#
if [ $# -lt 1 ]; then
    usage
fi

ARG_OPERATION=""
ARG_DOCUMENT="ALL"
ARG_FORMAT="ALL"
ARG_BUILD_DIRECTORY="$PWD/build"

if [[ ! -z "${KONVEKTA_DOCUMENT_ROOT}" ]]; then
    ARG_BUILD_DIRECTORY=${KONVEKTA_DOCUMENT_ROOT}
fi

#
# Check mandatory parameter
#
if [ $1 = "help" ]; then 
    usage 
elif [ $1 =  "build" ]; then
    ARG_OPERATION="BUILD"
elif [ $1 = "clean" ]; then
    ARG_OPERATION="CLEAN"	
else
    echo "Unknown argument $1"
    usage
fi
shift


#
# Check optional parameter
#
while [ $# -gt 0 ]; do
    
    if [ $# -lt 2 ]; then
        echo "missing arument after $1"
	usage
    fi

    case $1 in
        -d)
	    ARG_DOCUMENT=$2
	    shift
	    shift
	    ;;
        -f)
	    if [ $2 != "HTML" ] && [ $2 != "PDF" ]; then
	        echo "unknwon format $2"
		exit 2
	    fi 
	    ARG_FORMAT=$2
	    shift
	    shift
	    ;;
        -b)
            ARG_BUILD_DIRECTORY=$2
	    shift
	    shift
	    ;;
        -*)
	    usage
	    ;;
    esac
done

#
# Check build directory
#
if [ ! -d $ARG_BUILD_DIRECTORY ]; then
    echo "Directory $ARG_BUILD_DIRECTORY not exist - create it"
    mkdir $ARG_BUILD_DIRECTORY
fi
if [ ! -w $ARG_BUILD_DIRECTORY ]; then
    echo "Directory $ARG_BUILD_DIRECTORY is not writeable"
    exit 3
fi

#
# Create source directory list 
# 
SOURCE_DIRECTORY_LIST=()
if [ $ARG_DOCUMENT = "ALL" ]; then
    for directory in $(ls -d */); do 
	DIRECTORY_ALLOWED="YES"
	for forbidden_directory in "${SOURCE_DIRECTORY_BLACK_LIST[@]}"; do
	    if [ $directory = $forbidden_directory ]; then 
		DIRECTORY_ALLOWED="NO"
            fi
	done
	if [ $DIRECTORY_ALLOWED = "YES" ]; then
		SOURCE_DIRECTORY_LIST+=($directory)
	fi
    done 
else
    SOURCE_DIRECTORY_LIST+=$ARG_DOCUMENT
fi

#
# Create document format list
# 
DOCUMENT_FORMAT_LIST=()
if [ $ARG_FORMAT = "ALL" ]; then
    DOCUMENT_FORMAT_LIST=("html" "simplepdf")
elif [ $ARG_FORMAT = "HTML" ]; then
    DOCUMENT_FORMAT_LIST=("html")
elif [ $ARG_FORMAT = "PDF" ]; then
    DOCUMENT_FORMAT_LIST=("simplepdf")
fi	

echo "Build Operation:    $ARG_OPERATION"
echo "Build Document:     $ARG_DOCUMENT"
echo "Build Format:       $ARG_FORMAT"
echo "Build Directory:    $ARG_BUILD_DIRECTORY"
echo ""
echo "Source Directories:"
for directory in "${SOURCE_DIRECTORY_LIST[@]}"; do
    echo "    $directory"
done 
echo "Document Format:"
for format in "${DOCUMENT_FORMAT_LIST[@]}"; do
    echo "    $format"
done	

#
# Create documents
#
if [ $ARG_OPERATION = "BUILD" ]; then
    echo ""
    echo "Build ..."
    for directory in "${SOURCE_DIRECTORY_LIST[@]}"; do
        for format in "${DOCUMENT_FORMAT_LIST[@]}"; do
            echo "Create document:"
	    echo "    Directory: $directory"
	    echo "    Format: $format"

	    rm -rf tmp/
	    bin/DocumentationBuilder -M $format $directory tmp/ 

	    if [ $format = "simplepdf" ]; then
	        mkdir -p $ARG_BUILD_DIRECTORY/$directory/pdf
                cp tmp/simplepdf/*.pdf $ARG_BUILD_DIRECTORY/$directory/pdf
	    elif [ $format = "html" ]; then
		mkdir -p $ARG_BUILD_DIRECTORY/$directory/html
		cp -r tmp/html/* $ARG_BUILD_DIRECTORY/$directory/html
            fi

	    rm -rf tmp/
        done
    done


#
# Cleanup documents
#
elif [ $ARG_OPERATION = "CLEAN" ]; then
    echo ""
    echo "Cleanup ..."
    for directory in "${SOURCE_DIRECTORY_LIST[@]}"; do
        for format in "${DOCUMENT_FORMAT_LIST[@]}"; do
            echo "Cleanup document:"
	    echo "    Directory: $ARG_BUILD_DIRECTORY/$directory"
	    echo "    Format: $format"

	    if [ $format = "simplepdf" ]; then
	        rm -rf $ARG_BUILD_DIRECTORY/$directory/pdf
	    elif [ $format = "html" ]; then
	        rm -rf $ARG_BUILD_DIRECTORY/$directory/html
	    fi

	    if [ ! $(ls -A $ARG_BUILD_DIRECTORY/$directory) ]; then
	        rm -rf $ARG_BUILD_DIRECTORY/$directory
	    fi

	done
    done
fi

