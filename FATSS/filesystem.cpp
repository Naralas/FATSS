#include "filesystem.h"

FileSystem::FileSystem(int Size)
{
    fat = new QVector<QPair<int, int>>();
    rootDir = new QVector<QPair<QString, int>>();

    isReady = false;
    size = Size;

    if(Size < 12) // < 16 MB - doesn't work
    {

    }
    else if (Size < 64) // < 64MB
    {
        //512b
        clusterSize = 512;
    }
    else if (Size < 128) // < 128MB
    {
        //1kb
        clusterSize = 1024;
    }
    else if (Size < 256) // < 256MB
    {
        //2kb
        clusterSize = 2048;
    }
    else if (Size < 8000) // < 8GB
    {
        //4kb
        clusterSize = 4096;
    }
    else if (Size < 16000) // < 16GB
    {
        //8kb
        clusterSize = 8192;
    }
    else if (Size < 32000) // < 32GB
    {
        //16kb
        clusterSize = 16384;
    }
    else if (Size < 200000) // < 2TB
    {
        //32kb
        clusterSize = 32768;
    }
    else // > 2TB - doesn't work
    {

    }

    freeClusters = Size / clusterSize;
    data = new QVector<int>(freeClusters);
}


///
/// \brief FileSystem::format: Prepare the volume for it to be used
///
void FileSystem::format()
{
    isReady = true;

    //fill the data so we can use replace() later
    data->fill(-1, freeClusters);

    //TODO - create sectors and their positions in the memory
    int reservedSector = 32;


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


    //Check if there's enough place
    int neededClusters = Size / clusterSize;
    if( freeClusters < neededClusters )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the rootDir
    QPair<QString, int> fileEntry(Name, index);
    rootDir->append(fileEntry);
    int entryNum = rootDir->indexOf(fileEntry);

    //Create the array containing the indexes of the free clusters to use
    QList<int> clusters;
    clusters[0] = index;
    int clustersIndex = 1;

    //Find all clusters
    for(int i = index; i < fat->length() && clustersIndex < neededClusters; i++)
    {
        if(fat->at(i).first == 0)
        {
            clusters[clustersIndex] = i;
            clustersIndex++;
        }
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < neededClusters-1; i++)
    {
        fat->insert(clusters[i], QPair<int, int>(1, clusters[i+1]));
        data->replace(i, entryNum);
    }
    fat->insert(clusters[neededClusters-1], QPair<int, int>(1, -1));

    //Updates the free clusters counter
    freeClusters -= neededClusters;

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
