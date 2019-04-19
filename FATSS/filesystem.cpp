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
QString FileSystem::createFile(QString name, int Size)
{

    //Check if there's enough place
    int neededClusters = Size;
    if( freeClusters < neededClusters )
        return "The volume is full";

    //Check for the first free place
    int index = findFreeCluster(0);
    //Add it to the rootDir
    QPair<QString, int> fileEntry(name, index);
    rootDir->append(fileEntry);

    //Create the array containing the indexes of the free clusters to use
    QList<int>* clusters = new QList<int>();
    clusters->append(index);
    int lastClusters = index+1;

    //Find all free clusters
    while(clusters->size() < neededClusters)
    {
        lastClusters = findFreeCluster(lastClusters);
        clusters->append(lastClusters++);
    }

    //Fill the FAT array with the indexes and insert the last index
    for(int i = 0; i < clusters->size()-1; i++)
    {
        fat->at(clusters->at(i))->setVals(clusters->at(i+1), 1);
    }
    fat->at(clusters->at(neededClusters-1))->setVals(-1, 1);

    //Updates the free clusters counter
    freeClusters -= neededClusters;
     qDebug() << "clusters in fs: ";
    for(int i = 0; i< clusters->size(); i++)
    {
        qDebug() << " , " << clusters->at(i);
    }

    //For the signal
    FileEntry* file = new FileEntry(name, clusters);
    emit createdFile(file);
    qDebug() << "EMITTED";
    return "File successfully created";
}


QString FileSystem::updateFile(QString name, int newSize)
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

    //Run through the existing size of the file
    //If the Newsize is smaller than the actual size -> continue and unset the clusters
    //that are beyond the new size until reaching a cluster that link to "-1"

    //If the NewSize is larger than the actual size -> after the size is run through,
    //continue adding clusters to the chain until the NewSize is attained

    bool isLarger = false;
    int size;
    // Check if it's a reduction or an inflation
    for(size = 0; size < newSize; size++) //Run through while we haven't reached the newSize
    {
        //indexCluster is the pointer on the current cluster in the fat
        if(fat->at(indexCluster)->nextEntry == -1)
        {
            //We go here if we haven't reached the newSize and the size is already reached
            //It means the actual size is smaller than NewSize, so inflation
            isLarger = true;
            break;
        }

    }//If we finish the for loop without entering the condition, it means the size is at least equal to the NewSize, so it's a reduction



    //Inflation
    if(isLarger)
    {
        //Check if the newsize is within the free clusters
        if(freeClusters < newSize-size )
            return "Not enough space left";

        int nextCluster;
        //While the size is smaller than the newsize, we keep adding to the chain
        while(size < newSize)
        {
            freeClusters--;
            //Find next cluster
            nextCluster = findFreeCluster(indexCluster);

            //update current cluster
            fat->at(indexCluster)->setVals(nextCluster, 1);
            indexCluster = nextCluster;
            size++;
        }

        fat->at(indexCluster)->setVals(-1, 1);
    }
    else //Reduction
    {
        //While we're not at the end of the file, unset the clusters' nextentry
        while(fat->at(indexCluster)->state == 1 && fat->at(indexCluster)->nextEntry != 0)
        {
            fat->at(indexCluster)->state = 0;
            indexCluster = fat->at(indexCluster)->nextEntry;
        }
    }
    //FileEntry* file = new FileEntry(name, clusters);
    //emit updatedFile(file);
    return "Done";
}

QString FileSystem::defragmentation()
{
    //Go through all of the clusters
    int currentFile = -1;
    int nextClusters = -1;
    QList<int> defragmentedFile();
    for(int i = 0; i < fat->length(); i++)
    {
        //if(currentFile == -1);

        //Check current Cluster: Empty or filled
        //If we're currently defragmenting a file, and the cluster is empty
            //Take the next cluster of the file, and move it to the current place then update the last cluster pointer
        //If we're currently defragmenting a file, but the cluster is not empty
            //Take the cluster currently residing in it, and move it to the list and *problem* update the last cluster of the file
            //Then do the above step
        //If we're not defragmenting a file, and the cluster is empty, mark the place as first empty place
            //Then continue searching for the first cluster of a file
        //If we're not defragmenting a file, and the cluster is not empty *Problem* when the cluster is not the starting cluster


    }
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
    for(int i = index; i < fat->count(); i++)
    {
        if(fat->at(i)->state == 0)
        {
            return i;
        }
    }


    return -1;
}
