#include "filesystem.h"

FileSystem::FileSystem(int Size)
{
    rootDir = new QVector<QPair<QString, int>>();
    this->fat = new QVector<FatEntry*>();
    for(int i = 0; i < Size; i++)
    {
        fat->insert(i, new FatEntry());
    }
    size = Size;

    //Size equals clusters

    freeClusters = Size;
    data = new QVector<int>(freeClusters);
}


///
/// \brief FileSystem::format: Prepare the volume for it to be used
///
void FileSystem::format()
{
    //fill the data so we can use replace() later
}


///
/// \brief FileSystem::createFile: Create a file with the given filename, and size
/// \param Name
/// \param Size
/// \return
///
QString FileSystem::createFile(QString Name, int Size)
{

    //Check if there's enough place
    int neededClusters = Size;
    if( freeClusters < neededClusters )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the rootDir
    QPair<QString, int> fileEntry(Name, index);
    rootDir->append(fileEntry);

    //Create the array containing the indexes of the free clusters to use
    QList<int> clusters;
    clusters.append(index);
    int lastClusters = index+1;

    //Find all free clusters
    while(clusters.size() < neededClusters)
    {
        lastClusters = findFreeCluster(lastClusters);
        clusters.append(lastClusters++);
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < clusters.size()-2; i++)
    {
        fat->at(clusters[i])->setVals(clusters[i+1], 1);
    }
    fat->at(neededClusters-1)->setVals(-1, 1);

    //Updates the free clusters counter
    freeClusters -= neededClusters;

    qDebug() << "clusters in fs: " << clusters;

    //For the signal
    FileEntry* file = new FileEntry(Name, &clusters);
    emit createdFile(file, FileEntryAction::INSERT);
    qDebug() << "EMITTED";
    return "File successfully created";
}


///
/// \brief FileSystem::delFile: Delete the file with the given filename.
/// \param name
///
QString FileSystem::delFile(QString name)
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
        return "No file with this name";

    //Unset the chaining clusters
    int lastIndex;
    do
    {
        lastIndex = indexCluster;
        indexCluster = fat->at(lastIndex)->nextEntry;
        fat->at(lastIndex)->setVals(-1,0);

    } while(fat->at(indexCluster)->state != -1);


    emit deletedFile(name);
    return "file " + name + " has been removed successfully";
}

FileSystem::~FileSystem()
{
    qDeleteAll(*fat);
    delete fat;
}


///
/// \brief FileSystem::findFreeCluster: Find the next free cluster starting at the given index
/// \param start
/// \return
///
int FileSystem::findFreeCluster(int index)
{
    int count = fat->count();
    for(int i = index; i < fat->count(); i++)
    {
        if(fat->at(i)->state == 0)
        {
            return i;
        }
    }


    return -1;
}
