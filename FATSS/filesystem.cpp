#include "filesystem.h"

FileSystem::FileSystem(int Size)
{
    data = new QVector<int>(Size);

    fat = new QVector<QPair<int, int>>();
    rootDir = new QVector<QPair<QString, int>>();

    isReady = false;
    freeClusters = 10;

    if(Size < 12) // < 16 MB - doesn't work
    {

    }
    else if (Size < 64) // < 64MB
    {
        //512b
    }
    else if (Size < 128) // < 128MB
    {
        //1kb
    }
    else if (Size < 256) // < 256MB
    {
        //2kb
    }
    else if (Size < 8000) // < 8GB
    {
        //4kb
    }
    else if (Size < 16000) // < 16GB
    {
        //8kb
    }
    else if (Size < 32000) // < 32GB
    {
        //16kb
    }
    else if (Size < 200000) // < 2TB
    {
        //32kb
    }
    else // > 2TB - doesn't work
    {

    }



}


///
/// \brief FileSystem::format: Prepare the volume for it to be used
///
void FileSystem::format()
{
    isReady = true;
    //TODO - create boot sector
}


///
/// \brief FileSystem::createFile: Create a file with the given filename, and size
/// \param Name
/// \param Size
/// \return
///
QString FileSystem::createFile(QString Name, int Size)
{
    //Check if volume is formatted
    if(!isReady)
        return "Volume is not ready";

    //TODO - Calculate the number of clusters to be used

    //Check if there's enough place
    if( freeClusters < Size )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the dtf
    QPair<QString, int> dtfEntry(Name, index);
    rootDir->append(dtfEntry);

    //Create the array containing the indexes of the free clusters
    QList<int> clusters;
    clusters[0] = index;
    int clustersIndex = 1;

    //Find all clusters
    for(int i = index; i < fat->length() && clustersIndex < Size; i++)
    {
        if(fat->at(i).first == 0)
        {
            clusters[clustersIndex] = i;
            clustersIndex++;
        }
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < Size-1; i++)
    {
        fat->insert(clusters[i], QPair<int, int>(1, clusters[i+1]));
    }
    fat->insert(clusters[Size-1], QPair<int, int>(1, -1));

    //Updates the free clusters counter
    freeClusters -= Size;

    //TODO - Update actual data

    return "File successfully created";
}


///
/// \brief FileSystem::delFile: Delete the file with the given filename.
/// \param name
///
void FileSystem::delFile(QString name)
{
    //Find file in root directory
    int indexCluster = -1;
    for(int i = 0; i < rootDir->length(); i++)
    {
        if(rootDir->at(i).first == name.toLower())
        {
            indexCluster = i;
        }
    }

    if(indexCluster == -1)
        return; //"No file with this name";

    //Unset the chaining clusters
    int lastIndex;
    do
    {
        lastIndex = indexCluster;
        indexCluster = fat->at(lastIndex).second;
        fat->replace(lastIndex, QPair<int, int>(0, -1));

    } while(fat->at(indexCluster).second != -1);

    return;// "file " + name + " has been removed successfully";
}


///
/// \brief FileSystem::findFreeCluster: Find the next free cluster starting at the given index
/// \param start
/// \return
///
int FileSystem::findFreeCluster(int index)
{
    for(int i = index; i < fat->length(); i++)
    {
        if(fat->at(i).first == 0)
        {
            return i;
        }
    }

    return -1;
}
