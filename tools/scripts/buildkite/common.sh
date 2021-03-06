#! /bin/bash

set -eo pipefail

function get_field(){
    local fname="$1"
    local field="$2"
    ret=$(cat $fname | grep $field | awk 'BEGIN{FS=" "}{print $2}' | awk 'BEGIN{FS="\""}{print $2}')
    printf "%s" "${ret}"
}
function get_version() {
    if [ -f cmake/modules/feelpp.version.cmake ]; then
        vfile=cmake/modules/feelpp.version.cmake
    else
        vfile=$(dirname $0)/../feel/cmake/modules/feelpp.version.cmake
    fi
    IFS=''
    major=$(get_field "$vfile" "VERSION_MAJOR ")
    minor=$(get_field "$vfile" "VERSION_MINOR ")
    micro=$(get_field "$vfile" "VERSION_MICRO ")
    prerelease=$(get_field "$vfile" "VERSION_PRERELEASE ")
    printf "v%s" "${major}.${minor}.${micro}${prerelease}"
}

tag_from_target() {
    splitfrom=(`echo "$1" | tr ":" "\n"`)
    fromos=${splitfrom[0]}
    fromtag=${splitfrom[1]}

    tools/scripts/buildkite/list.sh $2 $3 | grep "$2-$3-${fromos}-${fromtag}"  | while read line ; do
        tokens=($line)
        image=${tokens[0]}
        printf "%s" "${image}"
    done
}
extratags_from_target() {
    splitfrom=(`echo "$1" | tr ":" "\n"`)
    fromos=${splitfrom[0]}
    fromtag=${splitfrom[1]}
    
    tools/scripts/buildkite/list.sh $2 $3 | grep "$2-$3-${fromos}-${fromtag}"  | while read line ; do
        tokens=($line)
        extratags=${tokens[@]:5}
        printf "%s" "${extratags}" 
    done
}

# Combines a dockerfile template with a generated FROM line
dockerfile_from() {
    local dockerfile="$1"
    local from="$2"
    printf 'FROM %s\n%s' "$from" "$(<$dockerfile)"
}

FEELPP_VERSION=$(get_version)
#echo "Feel++ version: $FEELPP_VERSION"
