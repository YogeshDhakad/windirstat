// options.cpp - Implementation of CPersistence, COptions and CRegistryUser
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2005 Bernhard Seifert
// Copyright (C) 2004-2017 WinDirStat Team (windirstat.net)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "stdafx.h"
#include "windirstat.h"
#include "dirstatdoc.h"
#include <common/commonhelpers.h>
#include "options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
    static COptions _theOptions;

    const LPCTSTR sectionPersistence        = _T("persistence");
    const LPCTSTR entryShowFreeSpace        = _T("showFreeSpace");
    const LPCTSTR entryShowUnknown          = _T("showUnknown");
    const LPCTSTR entryShowFileTypes        = _T("showFileTypes");
    const LPCTSTR entryShowTreemap          = _T("showTreemap");
    const LPCTSTR entryShowToolbar          = _T("showToolbar");
    const LPCTSTR entryShowStatusbar        = _T("showStatusbar");
    const LPCTSTR entryMainWindowPlacement  = _T("mainWindowPlacement");
    const LPCTSTR entrySplitterPosS         = _T("%s-splitterPos");
    const LPCTSTR entryColumnOrderS         = _T("%s-columnOrder");
    const LPCTSTR entryColumnWidthsS        = _T("%s-columnWidths");
    const LPCTSTR entryDialogRectangleS     = _T("%s-rectangle");
//     const LPCTSTR entrySortingColumnSN      = _T("%s-sortColumn%d");
//     const LPCTSTR entrySortingAscendingSN   = _T("%s-sortAscending%d");
    const LPCTSTR entryConfigPage           = _T("configPage");
    const LPCTSTR entryConfigPositionX      = _T("configPositionX");
    const LPCTSTR entryConfigPositionY      = _T("configPositionY");
    const LPCTSTR entrySelectDrivesRadio    = _T("selectDrivesRadio");
    const LPCTSTR entrySelectDrivesFolder   = _T("selectDrivesFolder");
    const LPCTSTR entrySelectDrivesDrives   = _T("selectDrivesDrives");
    const LPCTSTR entryShowDeleteWarning    = _T("showDeleteWarning");
    const LPCTSTR sectionBarState           = _T("persistence\\barstate");

    const LPCTSTR entryLanguage             = _T("language");

    const LPCTSTR sectionOptions            = _T("options");
    const LPCTSTR entryListGrid             = _T("treelistGrid"); // for compatibility with 1.0.1, this entry is named treelistGrid.
    const LPCTSTR entryListStripes          = _T("listStripes");
    const LPCTSTR entryListFullRowSelection = _T("listFullRowSelection");
    const LPCTSTR entryTreelistColorCount   = _T("treelistColorCount");
    const LPCTSTR entryTreelistColorN       = _T("treelistColor%d");
    const LPCTSTR entryHumanFormat          = _T("humanFormat");
    const LPCTSTR entryPacmanAnimation      = _T("pacmanAnimation");
    const LPCTSTR entryShowTimeSpent        = _T("showTimeSpent");
    const LPCTSTR entryTreemapHighlightColor= _T("treemapHighlightColor");
    const LPCTSTR entryTreemapStyle         = _T("treemapStyle");
    const LPCTSTR entryTreemapGrid          = _T("treemapGrid");
    const LPCTSTR entryTreemapGridColor     = _T("treemapGridColor");
    const LPCTSTR entryBrightness           = _T("brightness");
    const LPCTSTR entryHeightFactor         = _T("heightFactor");
    const LPCTSTR entryScaleFactor          = _T("scaleFactor");
    const LPCTSTR entryAmbientLight         = _T("ambientLight");
    const LPCTSTR entryLightSourceX         = _T("lightSourceX");
    const LPCTSTR entryLightSourceY         = _T("lightSourceY");
    const LPCTSTR entryFollowMountPoints    = _T("followMountPoints");
    const LPCTSTR entryFollowJunctionPoints = _T("followJunctionPoints");
    const LPCTSTR entrySkipHidden           = _T("skipHidden");
    const LPCTSTR entryUseWdsLocale         = _T("useWdsLocale");

    const LPCTSTR sectionUserDefinedCleanupD= _T("options\\userDefinedCleanup%02d");
    const LPCTSTR entryEnabled              = _T("enabled");
    const LPCTSTR entryTitle                = _T("title");
    const LPCTSTR entryWorksForDrives       = _T("worksForDrives");
    const LPCTSTR entryWorksForDirectories  = _T("worksForDirectories");
    const LPCTSTR entryWorksForFilesFolder  = _T("worksForFilesFolder");
    const LPCTSTR entryWorksForFiles        = _T("worksForFiles");
    const LPCTSTR entryWorksForUncPaths     = _T("worksForUncPaths");
    const LPCTSTR entryCommandLine          = _T("commandLine");
    const LPCTSTR entryRecurseIntoSubdirectories = _T("recurseIntoSubdirectories");
    const LPCTSTR entryAskForConfirmation   = _T("askForConfirmation");
    const LPCTSTR entryShowConsoleWindow    = _T("showConsoleWindow");
    const LPCTSTR entryWaitForCompletion    = _T("waitForCompletion");
    const LPCTSTR entryRefreshPolicy        = _T("refreshPolicy");
    const LPCTSTR entryReportSubject        = _T("reportSubject");
    const LPCTSTR entryReportPrefix         = _T("reportPrefix");
    const LPCTSTR entryReportSuffix         = _T("reportSuffix");

    COLORREF treelistColorDefault[TREELISTCOLORCOUNT] = {
        RGB(64, 64, 140),
        RGB(140, 64, 64),
        RGB(64, 140, 64),
        RGB(140, 140, 64),
        RGB(0, 0, 255),
        RGB(255, 0, 0),
        RGB(0, 255, 0),
        RGB(255, 255, 0)
    };

}

/////////////////////////////////////////////////////////////////////////////
bool CPersistence::GetShowFreeSpace()
{
    return getProfileBool(sectionPersistence, entryShowFreeSpace, false);
}

void CPersistence::SetShowFreeSpace(bool show)
{
    getProfileBool(sectionPersistence, entryShowFreeSpace, show);
}

bool CPersistence::GetShowUnknown()
{
    return getProfileBool(sectionPersistence, entryShowUnknown, false);
}

void CPersistence::SetShowUnknown(bool show)
{
    getProfileBool(sectionPersistence, entryShowUnknown, show);
}

bool CPersistence::GetShowFileTypes()
{
    return getProfileBool(sectionPersistence, entryShowFileTypes, true);
}

void CPersistence::SetShowFileTypes(bool show)
{
    getProfileBool(sectionPersistence, entryShowFileTypes, show);
}

bool CPersistence::GetShowTreemap()
{
    return getProfileBool(sectionPersistence, entryShowTreemap, true);
}

void CPersistence::SetShowTreemap(bool show)
{
    getProfileBool(sectionPersistence, entryShowTreemap, show);
}

bool CPersistence::GetShowToolbar()
{
    return getProfileBool(sectionPersistence, entryShowToolbar, true);
}

void CPersistence::SetShowToolbar(bool show)
{
    getProfileBool(sectionPersistence, entryShowToolbar, show);
}

bool CPersistence::GetShowStatusbar()
{
    return getProfileBool(sectionPersistence, entryShowStatusbar, true);
}

void CPersistence::SetShowStatusbar(bool show)
{
    getProfileBool(sectionPersistence, entryShowStatusbar, show);
}

void CPersistence::GetMainWindowPlacement(/* [in/out] */ WINDOWPLACEMENT& wp)
{
    ASSERT(wp.length == sizeof(wp));
    CString s = getProfileString(sectionPersistence, entryMainWindowPlacement);
    DecodeWindowPlacement(s, wp);
    CRect rc(wp.rcNormalPosition);
    SanitizeRect(rc);
    wp.rcNormalPosition = rc;
}

void CPersistence::SetMainWindowPlacement(const WINDOWPLACEMENT& wp)
{
    CString s = EncodeWindowPlacement(wp);
    setProfileString(sectionPersistence, entryMainWindowPlacement, s);
}

void CPersistence::SetSplitterPos(LPCTSTR name, bool valid, double userpos)
{
    int pos;
    if(valid)
    {
        pos = (int)(userpos * 100);
    }
    else
    {
        pos = -1;
    }

    setProfileInt(sectionPersistence, MakeSplitterPosEntry(name), pos);
}

void CPersistence::GetSplitterPos(LPCTSTR name, bool& valid, double& userpos)
{
    int pos = getProfileInt(sectionPersistence, MakeSplitterPosEntry(name), -1);
    if(pos < 0 || pos > 100)
    {
        valid = false;
        userpos = 0.5;
    }
    else
    {
        valid = true;
        userpos = (double)pos / 100;
    }
}

void CPersistence::SetColumnOrder(LPCTSTR name, const CArray<int, int>& arr)
{
    SetArray(MakeColumnOrderEntry(name), arr);
}

void CPersistence::GetColumnOrder(LPCTSTR name, /* in/out */ CArray<int, int>& arr)
{
    GetArray(MakeColumnOrderEntry(name), arr);
}

void CPersistence::SetColumnWidths(LPCTSTR name, const CArray<int, int>& arr)
{
    SetArray(MakeColumnWidthsEntry(name), arr);
}

void CPersistence::GetColumnWidths(LPCTSTR name, /* in/out */ CArray<int, int>& arr)
{
    GetArray(MakeColumnWidthsEntry(name), arr);
}

void CPersistence::SetDialogRectangle(LPCTSTR name, const CRect& rc)
{
    SetRect(MakeDialogRectangleEntry(name), rc);
}

void CPersistence::GetDialogRectangle(LPCTSTR name, CRect& rc)
{
    GetRect(MakeDialogRectangleEntry(name), rc);
    SanitizeRect(rc);
}
/*
void CPersistence::SetSorting(LPCTSTR name, int column1, bool ascending1, int column2, bool ascending2)
{
    setProfileInt(sectionPersistence, MakeSortingColumnEntry(name, 1), column1);
    getProfileBool(sectionPersistence, MakeSortingAscendingEntry(name, 1), ascending1);
    setProfileInt(sectionPersistence, MakeSortingColumnEntry(name, 2), column2);
    getProfileBool(sectionPersistence, MakeSortingAscendingEntry(name, 2), ascending2);
}

void CPersistence::GetSorting(LPCTSTR name, int columnCount, int& column1, bool& ascending1, int& column2, bool& ascending2)
{
    column1 = getProfileInt(sectionPersistence, MakeSortingColumnEntry(name, 1), column1);
    checkRange(column1, 0, columnCount - 1);
    ascending1 = getProfileBool(sectionPersistence, MakeSortingAscendingEntry(name, 1), ascending1);

    column2 = getProfileInt(sectionPersistence, MakeSortingColumnEntry(name, 2), column2);
    checkRange(column2, 0, columnCount - 1);
    ascending2 = getProfileBool(sectionPersistence, MakeSortingAscendingEntry(name, 2), ascending2);
}
*/

int CPersistence::GetConfigPage(int max)
{
    int n = getProfileInt(sectionPersistence, entryConfigPage, 0);
    checkRange(n, 0, max);
    return n;
}

void CPersistence::SetConfigPage(int page)
{
    setProfileInt(sectionPersistence, entryConfigPage, page);
}

void CPersistence::GetConfigPosition(/* in/out */ CPoint& pt)
{
    pt.x = getProfileInt(sectionPersistence, entryConfigPositionX, pt.x);
    pt.y = getProfileInt(sectionPersistence, entryConfigPositionY, pt.y);

    CRect rc(pt, CSize(100, 100));
    SanitizeRect(rc);
    pt = rc.TopLeft();
}

void CPersistence::SetConfigPosition(CPoint pt)
{
    setProfileInt(sectionPersistence, entryConfigPositionX, pt.x);
    setProfileInt(sectionPersistence, entryConfigPositionY, pt.y);
}

CString CPersistence::GetBarStateSection()
{
    return sectionBarState;
}

int CPersistence::GetSelectDrivesRadio()
{
    int radio = getProfileInt(sectionPersistence, entrySelectDrivesRadio, 0);
    checkRange(radio, 0, 2);
    return radio;
}

void CPersistence::SetSelectDrivesRadio(int radio)
{
    setProfileInt(sectionPersistence, entrySelectDrivesRadio, radio);
}

CString CPersistence::GetSelectDrivesFolder()
{
    return getProfileString(sectionPersistence, entrySelectDrivesFolder);
}

void CPersistence::SetSelectDrivesFolder(LPCTSTR folder)
{
    setProfileString(sectionPersistence, entrySelectDrivesFolder, folder);
}

void CPersistence::GetSelectDrivesDrives(CStringArray& drives)
{
    drives.RemoveAll();
    CString s = getProfileString(sectionPersistence, entrySelectDrivesDrives);
    int i = 0;
    while(i < s.GetLength())
    {
        CString drive;
        while(i < s.GetLength() && s[i] != wds::chrPipe)
        {
            drive += s[i];
            i++;
        }
        if(i < s.GetLength())
        {
            i++;
        }
        drives.Add(drive);
    }
}

void CPersistence::SetSelectDrivesDrives(const CStringArray& drives)
{
    CString s;
    for(int i = 0; i < drives.GetSize(); i++)
    {
        if(i > 0)
        {
            s += _T("|");
        }
        s += drives[i];
    }
    setProfileString(sectionPersistence, entrySelectDrivesDrives, s);
}

bool CPersistence::GetShowDeleteWarning()
{
    return getProfileBool(sectionPersistence, entryShowDeleteWarning, true);
}

void CPersistence::SetShowDeleteWarning(bool show)
{
    getProfileBool(sectionPersistence, entryShowDeleteWarning, show);
}

void CPersistence::SetArray(LPCTSTR entry, const CArray<int, int>& arr)
{
    CString value;
    for(int i = 0; i < arr.GetSize(); i++)
    {
        CString s;
        s.Format(_T("%d"), arr[i]);
        if(i > 0)
        {
            value += _T(",");
        }
        value += s;
    }
    setProfileString(sectionPersistence, entry, value);
}

void CPersistence::GetArray(LPCTSTR entry, /* in/out */ CArray<int, int>& rarr)
{
    CString s = getProfileString(sectionPersistence, entry);
    CArray<int, int> arr;
    int i = 0;
    while(i < s.GetLength())
    {
        int n = 0;
        while(i < s.GetLength() && _istdigit(s[i]))
        {
            n*= 10;
            n += s[i] - wds::chrZero;
            i++;
        }
        arr.Add(n);
        if(i >= s.GetLength() || s[i] != wds::chrComma)
            break;
        i++;
    }
    if(i >= s.GetLength() && arr.GetSize() == rarr.GetSize())
    {
        for(i = 0; i < rarr.GetSize(); i++)
            rarr[i]= arr[i];
    }
}

void CPersistence::SetRect(LPCTSTR entry, const CRect& rc)
{
    CString s;
    s.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
    setProfileString(sectionPersistence, entry, s);
}

void CPersistence::GetRect(LPCTSTR entry, CRect& rc)
{
    CString s = getProfileString(sectionPersistence, entry);
    CRect tmp;
    int r = _stscanf_s(s, _T("%d,%d,%d,%d"), &tmp.left, &tmp.top, &tmp.right, &tmp.bottom);
    if(r == 4)
    {
        rc = tmp;
    }
}

void CPersistence::SanitizeRect(CRect& rc)
{
    const int visible = 30;

    rc.NormalizeRect();

    CRect rcDesktop;
    CWnd::GetDesktopWindow()->GetWindowRect(rcDesktop);

    if(rc.Width() > rcDesktop.Width())
    {
        rc.right = rc.left + rcDesktop.Width();
    }
    if(rc.Height() > rcDesktop.Height())
    {
        rc.bottom = rc.top + rcDesktop.Height();
    }

    if(rc.left < 0)
    {
        rc.OffsetRect(-rc.left, 0);
    }
    if(rc.left > rcDesktop.right - visible)
    {
        rc.OffsetRect(-visible, 0);
    }

    if(rc.top < 0)
    {
        rc.OffsetRect(-rc.top, 0);
    }
    if(rc.top > rcDesktop.bottom - visible)
    {
        rc.OffsetRect(0, -visible);
    }
}

CString CPersistence::MakeSplitterPosEntry(LPCTSTR name)
{
    CString entry;
    entry.Format(entrySplitterPosS, name);
    return entry;
}

CString CPersistence::MakeColumnOrderEntry(LPCTSTR name)
{
    CString entry;
    entry.Format(entryColumnOrderS, name);
    return entry;
}

CString CPersistence::MakeDialogRectangleEntry(LPCTSTR name)
{
    CString entry;
    entry.Format(entryDialogRectangleS, name);
    return entry;
}

CString CPersistence::MakeColumnWidthsEntry(LPCTSTR name)
{
    CString entry;
    entry.Format(entryColumnWidthsS, name);
    return entry;
}

/*
CString CPersistence::MakeSortingColumnEntry(LPCTSTR name, int n)
{
    CString entry;
    entry.Format(entrySortingColumnSN, name, n);
    return entry;
}

CString CPersistence::MakeSortingAscendingEntry(LPCTSTR name, int n)
{
    CString entry;
    entry.Format(entrySortingAscendingSN, name, n);
    return entry;
}
*/

CString CPersistence::EncodeWindowPlacement(const WINDOWPLACEMENT& wp)
{
    CString s;
    s.Format(
        _T("%u,%u,")
        _T("%ld,%ld,%ld,%ld,")
        _T("%ld,%ld,%ld,%ld"),
        wp.flags, wp.showCmd,
        wp.ptMinPosition.x, wp.ptMinPosition.y, wp.ptMaxPosition.x, wp.ptMaxPosition.y,
        wp.rcNormalPosition.left, wp.rcNormalPosition.right, wp.rcNormalPosition.top, wp.rcNormalPosition.bottom
    );
    return s;
}

void CPersistence::DecodeWindowPlacement(const CString& s, WINDOWPLACEMENT& rwp)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);

    int r = _stscanf_s(s,
        _T("%u,%u,")
        _T("%ld,%ld,%ld,%ld,")
        _T("%ld,%ld,%ld,%ld"),
        &wp.flags, &wp.showCmd,
        &wp.ptMinPosition.x, &wp.ptMinPosition.y, &wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
        &wp.rcNormalPosition.left, &wp.rcNormalPosition.right, &wp.rcNormalPosition.top, &wp.rcNormalPosition.bottom
    );

    if(r == 10)
        rwp = wp;
}


/////////////////////////////////////////////////////////////////////////////

LANGID CLanguageOptions::GetLanguage()
{
    LANGID defaultLangid = LANGIDFROMLCID(GetUserDefaultLCID());
    LANGID id = (LANGID)getProfileInt(sectionOptions, entryLanguage, defaultLangid);
    return id;
}

void CLanguageOptions::SetLanguage(LANGID langid)
{
    setProfileInt(sectionOptions, entryLanguage, langid);
}


/////////////////////////////////////////////////////////////////////////////
COptions *GetOptions()
{
    return &_theOptions;
}


COptions::COptions()
{
}

bool COptions::IsListGrid()
{
    return m_listGrid;
}

void COptions::SetListGrid(bool show)
{
    if(m_listGrid != show)
    {
        m_listGrid = show;
        GetDocument()->UpdateAllViews(NULL, HINT_LISTSTYLECHANGED);
    }
}

bool COptions::IsListStripes()
{
    return m_listStripes;
}

void COptions::SetListStripes(bool show)
{
    if(m_listStripes != show)
    {
        m_listStripes = show;
        GetDocument()->UpdateAllViews(NULL, HINT_LISTSTYLECHANGED);
    }
}

bool COptions::IsListFullRowSelection()
{
    return m_listFullRowSelection;
}

void COptions::SetListFullRowSelection(bool show)
{
    if(m_listFullRowSelection != show)
    {
        m_listFullRowSelection = show;
        GetDocument()->UpdateAllViews(NULL, HINT_LISTSTYLECHANGED);
    }
}

void COptions::GetTreelistColors(COLORREF color[TREELISTCOLORCOUNT])
{
    for(int i = 0; i < TREELISTCOLORCOUNT; i++)
    {
        color[i]= m_treelistColor[i];
    }
}

void COptions::SetTreelistColors(const COLORREF color[TREELISTCOLORCOUNT])
{
    for(int i = 0; i < TREELISTCOLORCOUNT; i++)
    {
        m_treelistColor[i]= color[i];
    }
    GetDocument()->UpdateAllViews(NULL, HINT_LISTSTYLECHANGED);
}

COLORREF COptions::GetTreelistColor(int i)
{
    ASSERT(i >= 0);
    ASSERT(i < m_treelistColorCount);
    return m_treelistColor[i];
}

int COptions::GetTreelistColorCount()
{
    return m_treelistColorCount;
}

void COptions::SetTreelistColorCount(int count)
{
    if(m_treelistColorCount != count)
    {
        m_treelistColorCount = count;
        GetDocument()->UpdateAllViews(NULL, HINT_LISTSTYLECHANGED);
    }
}

bool COptions::IsHumanFormat()
{
    return m_humanFormat;
}

void COptions::SetHumanFormat(bool human)
{
    if(m_humanFormat != human)
    {
        m_humanFormat = human;
        GetDocument()->UpdateAllViews(NULL, HINT_NULL);
        GetWDSApp()->UpdateRamUsage();
    }
}

bool COptions::IsPacmanAnimation()
{
    return m_pacmanAnimation;
}

void COptions::SetPacmanAnimation(bool animate)
{
    if(m_pacmanAnimation != animate)
    {
        m_pacmanAnimation = animate;
    }
}

bool COptions::IsShowTimeSpent()
{
    return m_showTimeSpent;
}

void COptions::SetShowTimeSpent(bool show)
{
    if(m_showTimeSpent != show)
    {
        m_showTimeSpent = show;
    }
}

COLORREF COptions::GetTreemapHighlightColor()
{
    return m_treemapHighlightColor;
}

void COptions::SetTreemapHighlightColor(COLORREF color)
{
    if(m_treemapHighlightColor != color)
    {
        m_treemapHighlightColor = color;
        GetDocument()->UpdateAllViews(NULL, HINT_SELECTIONSTYLECHANGED);
    }
}

const CTreemap::Options *COptions::GetTreemapOptions()
{
    return &m_treemapOptions;
}

void COptions::SetTreemapOptions(const CTreemap::Options& options)
{
    if(options.style != m_treemapOptions.style
        || options.grid != m_treemapOptions.grid
        || options.gridColor != m_treemapOptions.gridColor
        || options.brightness != m_treemapOptions.brightness
        || options.height != m_treemapOptions.height
        || options.scaleFactor != m_treemapOptions.scaleFactor
        || options.ambientLight != m_treemapOptions.ambientLight
        || options.lightSourceX != m_treemapOptions.lightSourceX
        || options.lightSourceY != m_treemapOptions.lightSourceY
    )
    {
        m_treemapOptions = options;
        GetDocument()->UpdateAllViews(NULL, HINT_TREEMAPSTYLECHANGED);
    }
}

void COptions::GetUserDefinedCleanups(USERDEFINEDCLEANUP udc[USERDEFINEDCLEANUPCOUNT])
{
    for(int i = 0; i < USERDEFINEDCLEANUPCOUNT; i++)
    {
        udc[i]= m_userDefinedCleanup[i];
    }
}

void COptions::SetUserDefinedCleanups(const USERDEFINEDCLEANUP udc[USERDEFINEDCLEANUPCOUNT])
{
    for(int i = 0; i < USERDEFINEDCLEANUPCOUNT; i++)
    {
        m_userDefinedCleanup[i]= udc[i];
    }
}

void COptions::GetEnabledUserDefinedCleanups(CArray<int, int>& indices)
{
    indices.RemoveAll();
    for(int i = 0; i < USERDEFINEDCLEANUPCOUNT; i++)
    {
        if(m_userDefinedCleanup[i].enabled)
        {
            indices.Add(i);
        }
    }
}

bool COptions::IsUserDefinedCleanupEnabled(int i)
{
    ASSERT(i >= 0);
    ASSERT(i < USERDEFINEDCLEANUPCOUNT);
    return m_userDefinedCleanup[i].enabled;
}

const USERDEFINEDCLEANUP *COptions::GetUserDefinedCleanup(int i)
{
    ASSERT(i >= 0);
    ASSERT(i < USERDEFINEDCLEANUPCOUNT);
    ASSERT(m_userDefinedCleanup[i].enabled);

    return &m_userDefinedCleanup[i];
}

bool COptions::IsFollowMountPoints()
{
    return m_followMountPoints;
}

void COptions::SetFollowMountPoints(bool follow)
{
    if(m_followMountPoints != follow)
    {
        m_followMountPoints = follow;
        GetDocument()->RefreshMountPointItems();
    }
}

bool COptions::IsFollowJunctionPoints()
{
    return m_followJunctionPoints;
}

void COptions::SetFollowJunctionPoints(bool follow)
{
    if(m_followJunctionPoints != follow)
    {
        m_followJunctionPoints = follow;
        GetDocument()->RefreshJunctionItems();
    }
}

bool COptions::IsUseWdsLocale()
{
    return m_useWdsLocale;
}

void COptions::SetUseWdsLocale(bool use)
{
    if(m_useWdsLocale != use)
    {
        m_useWdsLocale = use;
        GetDocument()->UpdateAllViews(NULL, HINT_NULL);
    }
}

bool COptions::IsSkipHidden()
{
    return m_skipHidden;
}

void COptions::SetSkipHidden(bool skip)
{
    if(m_skipHidden != skip)
    {
        m_skipHidden = skip;
    }
}

CString COptions::GetReportSubject()
{
    return m_reportSubject;
}

CString COptions::GetReportDefaultSubject()
{
    return LoadString(IDS_REPORT_DISKUSAGE);
}

void COptions::SetReportSubject(LPCTSTR subject)
{
    m_reportSubject = subject;
}

CString COptions::GetReportPrefix()
{
    return m_reportPrefix;
}

CString COptions::GetReportDefaultPrefix()
{
    return LoadString(IDS_PLEASECHECKYOURDISKUSAGE);
}

void COptions::SetReportPrefix(LPCTSTR prefix)
{
    m_reportPrefix = prefix;
}

CString COptions::GetReportSuffix()
{
    return m_reportSuffix;
}

CString COptions::GetReportDefaultSuffix()
{
    CString suffix = LoadString(IDS_DISKUSAGEREPORTGENERATEDBYWINDIRSTAT);
    suffix.AppendFormat(_T("https://%s/\r\n"), GetWinDirStatHomepage().GetString());
    return suffix;
}

void COptions::SetReportSuffix(LPCTSTR suffix)
{
    m_reportSuffix = suffix;
}



void COptions::SaveToRegistry()
{
    int i = 0;
    setProfileBool(sectionOptions, entryListGrid, m_listGrid);
    setProfileBool(sectionOptions, entryListStripes, m_listStripes);
    setProfileBool(sectionOptions, entryListFullRowSelection, m_listFullRowSelection);

    setProfileInt(sectionOptions, entryTreelistColorCount, m_treelistColorCount);
    for(i  =  0; i < TREELISTCOLORCOUNT; i++)
    {
        CString entry;
        entry.Format(entryTreelistColorN, i);
        setProfileInt(sectionOptions, entry, m_treelistColor[i]);
    }
    setProfileBool(sectionOptions, entryHumanFormat, m_humanFormat);
    setProfileBool(sectionOptions, entrySkipHidden, m_skipHidden);
    setProfileBool(sectionOptions, entryPacmanAnimation, m_pacmanAnimation);
    setProfileBool(sectionOptions, entryShowTimeSpent, m_showTimeSpent);
    setProfileInt(sectionOptions, entryTreemapHighlightColor, m_treemapHighlightColor);

    SaveTreemapOptions();

    getProfileBool(sectionOptions, entryFollowMountPoints, m_followMountPoints);
    getProfileBool(sectionOptions, entryFollowJunctionPoints, m_followJunctionPoints);
    setProfileBool(sectionOptions, entryUseWdsLocale, m_useWdsLocale);

    for(i  =  0; i < USERDEFINEDCLEANUPCOUNT; i++)
    {
        SaveUserDefinedCleanup(i);
    }


    // We must distinguish between 'empty' and 'default'.
    // 'Default' will read ""
    // 'Empty' will read "$"
    // Others will read "$text.."
    const LPCTSTR stringPrefix = _T("$");

    CString s;

    if(m_reportSubject == GetReportDefaultSubject())
    {
        s.Empty();
    }
    else
    {
        s = stringPrefix + m_reportSubject;
    }
    setProfileString(sectionOptions, entryReportSubject, s);


    if(m_reportPrefix == GetReportDefaultPrefix())
    {
        s.Empty();
    }
    else
    {
        s = stringPrefix + m_reportPrefix;
    }
    setProfileString(sectionOptions, entryReportPrefix, s);


    if(m_reportSuffix == GetReportDefaultSuffix())
    {
        s.Empty();
    }
    else
    {
        s = stringPrefix + m_reportSuffix;
    }
    setProfileString(sectionOptions, entryReportSuffix, s);
}

void COptions::LoadFromRegistry()
{
    int i = 0;
    m_listGrid = getProfileBool(sectionOptions, entryListGrid, false);
    m_listStripes = getProfileBool(sectionOptions, entryListStripes, false);
    m_listFullRowSelection = getProfileBool(sectionOptions, entryListFullRowSelection, true);

    m_treelistColorCount = getProfileInt(sectionOptions, entryTreelistColorCount, 4);
    checkRange(m_treelistColorCount, 1, TREELISTCOLORCOUNT);
    for(i  =  0; i < TREELISTCOLORCOUNT; i++)
    {
        CString entry;
        entry.Format(entryTreelistColorN, i);
        m_treelistColor[i]= getProfileInt(sectionOptions, entry, treelistColorDefault[i]);
    }
    m_humanFormat = getProfileBool(sectionOptions, entryHumanFormat, true);
    m_skipHidden = getProfileBool(sectionOptions, entrySkipHidden, false);
    m_pacmanAnimation = getProfileBool(sectionOptions, entryPacmanAnimation, false);
    m_showTimeSpent = getProfileBool(sectionOptions, entryShowTimeSpent, false);
    m_treemapHighlightColor = getProfileInt(sectionOptions, entryTreemapHighlightColor, RGB(255,255,255));

    ReadTreemapOptions();

    m_followMountPoints = getProfileBool(sectionOptions, entryFollowMountPoints, false);
    // Ignore junctions by default
    m_followJunctionPoints = getProfileBool(sectionOptions, entryFollowJunctionPoints, false);
    // use user locale by default
    m_useWdsLocale = getProfileBool(sectionOptions, entryUseWdsLocale, false);

    for(i = 0; i < USERDEFINEDCLEANUPCOUNT; i++)
    {
        ReadUserDefinedCleanup(i);
    }


    CString s = getProfileString(sectionOptions, entryReportSubject);
    if(s.IsEmpty())
    {
        m_reportSubject = GetReportDefaultSubject();
    }
    else
    {
        m_reportSubject = s.Mid(1);
    }

    s = getProfileString(sectionOptions, entryReportPrefix);
    if(s.IsEmpty())
    {
        m_reportPrefix = GetReportDefaultPrefix();
    }
    else
    {
        m_reportPrefix = s.Mid(1);
    }

    s = getProfileString(sectionOptions, entryReportSuffix);
    if(s.IsEmpty())
    {
        m_reportSuffix = GetReportDefaultSuffix();
    }
    else
    {
        m_reportSuffix = s.Mid(1);
    }
}

void COptions::ReadUserDefinedCleanup(int i)
{
    CString section;
    section.Format(sectionUserDefinedCleanupD, i);

    CString defaultTitle;
    defaultTitle.FormatMessage(IDS_USERDEFINEDCLEANUPd, i);

    m_userDefinedCleanup[i].enabled = getProfileBool(section, entryEnabled, false);
    m_userDefinedCleanup[i].title = getProfileString(section, entryTitle);
    if(m_userDefinedCleanup[i].title.IsEmpty())
    {
        m_userDefinedCleanup[i].title = defaultTitle;
        m_userDefinedCleanup[i].virginTitle = true;
    }
    else
    {
        m_userDefinedCleanup[i].virginTitle = false;
    }
    m_userDefinedCleanup[i].worksForDrives = getProfileBool(section, entryWorksForDrives, false);
    m_userDefinedCleanup[i].worksForDirectories = getProfileBool(section, entryWorksForDirectories, false);
    m_userDefinedCleanup[i].worksForFilesFolder = getProfileBool(section, entryWorksForFilesFolder, false);
    m_userDefinedCleanup[i].worksForFiles = getProfileBool(section, entryWorksForFiles, false);
    m_userDefinedCleanup[i].worksForUncPaths = getProfileBool(section, entryWorksForUncPaths, false);
    m_userDefinedCleanup[i].commandLine = getProfileString(section, entryCommandLine);
    m_userDefinedCleanup[i].recurseIntoSubdirectories = getProfileBool(section, entryRecurseIntoSubdirectories, false);
    m_userDefinedCleanup[i].askForConfirmation = getProfileBool(section, entryAskForConfirmation, true);
    m_userDefinedCleanup[i].showConsoleWindow = getProfileBool(section, entryShowConsoleWindow, true);
    m_userDefinedCleanup[i].waitForCompletion = getProfileBool(section, entryWaitForCompletion, true);
    int r = getProfileInt(section, entryRefreshPolicy, RP_NO_REFRESH);
    checkRange(r, 0, REFRESHPOLICYCOUNT);
    m_userDefinedCleanup[i].refreshPolicy = (REFRESHPOLICY)r;
}

void COptions::SaveUserDefinedCleanup(int i)
{
    CString section;
    section.Format(sectionUserDefinedCleanupD, i);

    getProfileBool(section, entryEnabled, m_userDefinedCleanup[i].enabled);
    if(m_userDefinedCleanup[i].virginTitle)
    {
        setProfileString(section, entryTitle, wds::strEmpty);
    }
    else
    {
        setProfileString(section, entryTitle, m_userDefinedCleanup[i].title);
    }
    getProfileBool(section, entryWorksForDrives, m_userDefinedCleanup[i].worksForDrives);
    getProfileBool(section, entryWorksForDirectories, m_userDefinedCleanup[i].worksForDirectories);
    getProfileBool(section, entryWorksForFilesFolder, m_userDefinedCleanup[i].worksForFilesFolder);
    getProfileBool(section, entryWorksForFiles, m_userDefinedCleanup[i].worksForFiles);
    getProfileBool(section, entryWorksForUncPaths, m_userDefinedCleanup[i].worksForUncPaths);
    setProfileString(section, entryCommandLine, m_userDefinedCleanup[i].commandLine);
    getProfileBool(section, entryRecurseIntoSubdirectories, m_userDefinedCleanup[i].recurseIntoSubdirectories);
    getProfileBool(section, entryAskForConfirmation, m_userDefinedCleanup[i].askForConfirmation);
    getProfileBool(section, entryShowConsoleWindow, m_userDefinedCleanup[i].showConsoleWindow);
    getProfileBool(section, entryWaitForCompletion, m_userDefinedCleanup[i].waitForCompletion);
    setProfileInt(section, entryRefreshPolicy, m_userDefinedCleanup[i].refreshPolicy);
}

void COptions::ReadTreemapOptions()
{
    CTreemap::Options standard = CTreemap::GetDefaultOptions();

    int style = getProfileInt(sectionOptions, entryTreemapStyle, standard.style);
    if(style != CTreemap::KDirStatStyle && style != CTreemap::SequoiaViewStyle)
    {
        style = CTreemap::KDirStatStyle;
    }
    m_treemapOptions.style = (CTreemap::STYLE)style;

    m_treemapOptions.grid = getProfileBool(sectionOptions, entryTreemapGrid, standard.grid);

    m_treemapOptions.gridColor = getProfileInt(sectionOptions, entryTreemapGridColor, standard.gridColor);

    int brightness = getProfileInt(sectionOptions, entryBrightness, standard.GetBrightnessPercent());
    checkRange(brightness, 0, 100);
    m_treemapOptions.SetBrightnessPercent(brightness);

    int height = getProfileInt(sectionOptions, entryHeightFactor, standard.GetHeightPercent());
    checkRange(height, 0, 100);
    m_treemapOptions.SetHeightPercent(height);

    int scaleFactor = getProfileInt(sectionOptions, entryScaleFactor, standard.GetScaleFactorPercent());
    checkRange(scaleFactor, 0, 100);
    m_treemapOptions.SetScaleFactorPercent(scaleFactor);

    int ambientLight = getProfileInt(sectionOptions, entryAmbientLight, standard.GetAmbientLightPercent());
    checkRange(ambientLight, 0, 100);
    m_treemapOptions.SetAmbientLightPercent(ambientLight);

    int lightSourceX = getProfileInt(sectionOptions, entryLightSourceX, standard.GetLightSourceXPercent());
    checkRange(lightSourceX, -200, 200);
    m_treemapOptions.SetLightSourceXPercent(lightSourceX);

    int lightSourceY = getProfileInt(sectionOptions, entryLightSourceY, standard.GetLightSourceYPercent());
    checkRange(lightSourceY, -200, 200);
    m_treemapOptions.SetLightSourceYPercent(lightSourceY);
}

void COptions::SaveTreemapOptions()
{
    setProfileInt(sectionOptions, entryTreemapStyle, m_treemapOptions.style);
    getProfileBool(sectionOptions, entryTreemapGrid, m_treemapOptions.grid);
    setProfileInt(sectionOptions, entryTreemapGridColor, m_treemapOptions.gridColor);
    setProfileInt(sectionOptions, entryBrightness, m_treemapOptions.GetBrightnessPercent());
    setProfileInt(sectionOptions, entryHeightFactor, m_treemapOptions.GetHeightPercent());
    setProfileInt(sectionOptions, entryScaleFactor, m_treemapOptions.GetScaleFactorPercent());
    setProfileInt(sectionOptions, entryAmbientLight, m_treemapOptions.GetAmbientLightPercent());
    setProfileInt(sectionOptions, entryLightSourceX, m_treemapOptions.GetLightSourceXPercent());
    setProfileInt(sectionOptions, entryLightSourceY, m_treemapOptions.GetLightSourceYPercent());
}


/////////////////////////////////////////////////////////////////////////////

void CRegistryUser::setProfileString(LPCTSTR section, LPCTSTR entry, LPCTSTR value)
{
    AfxGetApp()->WriteProfileString(section, entry, value);
}

CString CRegistryUser::getProfileString(LPCTSTR section, LPCTSTR entry, LPCTSTR defaultValue)
{
    return AfxGetApp()->GetProfileString(section, entry, defaultValue);
}

void CRegistryUser::setProfileInt(LPCTSTR section, LPCTSTR entry, int value)
{
    AfxGetApp()->WriteProfileInt(section, entry, value);
}

int CRegistryUser::getProfileInt(LPCTSTR section, LPCTSTR entry, int defaultValue)
{
    return AfxGetApp()->GetProfileInt(section, entry, defaultValue);
}

void CRegistryUser::setProfileBool(LPCTSTR section, LPCTSTR entry, bool value)
{
    setProfileInt(section, entry, (int)value);
}

bool CRegistryUser::getProfileBool(LPCTSTR section, LPCTSTR entry, bool defaultValue)
{
    return getProfileInt(section, entry, defaultValue) != 0;
}

void CRegistryUser::checkRange(int& value, int min, int max)
{
    if(value < min)
    {
        value = min;
    }
    if(value > max)
    {
        value = max;
    }
}
