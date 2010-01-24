#! /bin/sh

VERSION=1.6.0
RPMBUILDPATH=../../../rpmbuild/

echo "Build source TGZ:"
make dist

echo ""
echo ""
echo ""
echo ""
echo ""
echo "Build Debian packages:"
echo "- Edit debian/copyright"
echo "- Edit debian/changelog"
echo "- Edit debian/control"
echo "- Edit debian/rules"
fakeroot debian/rules binary

echo ""
echo ""
echo ""
echo ""
echo ""
echo "Build RPM packages:"
echo "- Edit Utils.spec"
#We copy the source directory in the rpmbuild one... 
cp bpp-seq-$VERSION.tar.gz $RPMBUILDPATH/SOURCES/
rpmbuild -ba Seq.spec

echo "Done."

