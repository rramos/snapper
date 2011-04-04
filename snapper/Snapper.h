/*
 * Copyright (c) 2011 Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef SNAPPER_H
#define SNAPPER_H


#include <vector>

#include "snapper/Snapshot.h"


namespace snapper
{
    using std::vector;


    class SysconfigFile;


    struct CompareCallback
    {
	CompareCallback() {}
	virtual ~CompareCallback() {}

	virtual void start() {}
	virtual void stop() {}
    };


    class Snapper
    {
    public:

	Snapper(const string& config_name = "root");
	~Snapper();

	string subvolumeDir() const;
	string snapshotsDir() const;

	Snapshots& getSnapshots() { return snapshots; }
	const Snapshots& getSnapshots() const { return snapshots; }

	Snapshots::const_iterator getSnapshotCurrent() const;

	Snapshots::iterator createSingleSnapshot(string description);
	Snapshots::iterator createPreSnapshot(string description);
	Snapshots::iterator createPostSnapshot(Snapshots::const_iterator pre);

	void deleteSnapshot(Snapshots::iterator snapshot);

	void startBackgroundComparsion(Snapshots::const_iterator snapshot1,
				       Snapshots::const_iterator snapshot2);

	bool doCleanupNumber();
	bool doCleanupTimeline();
	bool doCleanupEmptyPrePost();

	void setCompareCallback(CompareCallback* p) { compare_callback = p; }
	CompareCallback* getCompareCallback() const { return compare_callback; }

	const vector<string>& getFilterPatterns() const { return filter_patterns; }

    private:

	void filter1(list<Snapshots::iterator>& tmp, time_t min_age);
	void filter2(list<Snapshots::iterator>& tmp);

	void loadPatterns();

	const string config_name;

	SysconfigFile* config;

	string subvolume;

	vector<string> filter_patterns;

	Snapshots snapshots;

	CompareCallback* compare_callback;

    };

};


#endif
